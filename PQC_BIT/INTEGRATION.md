# BiT-128 接入与验证

本模块采用与其他 PQC 目录一致的布局，只提供 128 位安全等级的普通密钥生成、签名和验签。

| 目录 | 内容 |
|---|---|
| `include/` | 公共 API、参数、直接 OP 随机接口 |
| `core/common/` | 普通版 C 算法与 OP 哈希、NTT 封装 |
| `platform/pc/` | Make/CMake 构建、算子接口和测试 |
| `platform/pc/soft/` | PC 软件 OP 后端 |


原始发布目录位于仓库根下的 `local-reference/`，已加入 `.gitignore`，仅本地保留。
其中 256 位、AVX、预计算接口均不参与本模块构建。原 KAT 的实例名
`BiP-128` 保留以便直接逐字节比较。

## 构建与验证

```sh
make -C PQC_BIT/platform/pc
make -C PQC_BIT/platform/pc check memory stack kat
make -C PQC_BIT/platform/pc sanitize
make -C PQC_BIT/platform/pc benchmark
```

O3 使用独立目录构建，并让原版与生产适配版都使用 O3 计时：

```sh
make -C PQC_BIT/platform/pc BUILD=build-ntt-o3 \
  CFLAGS='-O3 -std=c11 -Wall -Wextra -Wpedantic' check memory stack kat
python3 PQC_BIT/platform/pc/test/run_comparison.py --opt-level O3 --rounds 9 \
  --cc clang --output PQC_BIT/platform/pc/build-ntt-o3/comparison.json
```

`check/memory/stack/kat/sanitize` 无需外部原始发布目录。`benchmark` 需要本地
原版，默认位置为 `local-reference/BiP-release-20261030/`；全新克隆后可自行
放入原版，或指定其他路径：

```sh
python3 PQC_BIT/platform/pc/test/run_comparison.py \
  --reference /path/to/BiP-release-20261030 --opt-level O3 --rounds 9
```

静态库为 `PQC_BIT/platform/pc/build/libpqc_bit.a`。`check` 包含原版完整正确性
测试（确定性测试 OP 和真实随机 OP 两种模式）及哈希、随机、NTT 的 OP 故障注入；`memory` 检查堆峰值、分配失败、释放前清零；`stack`
重新生成汇编和 `.su` 文件，检查已知 C 调用链；`benchmark` 用同一个驱动
构建原版、确定性 OP 测试版和真实 OP 生产版。确定性测试版检查 512 份
签名字节一致；生产版做双向验签和双向跨私钥签名，再与原版交替计时。

```sh
cmake -S PQC_BIT/platform/pc -B /tmp/bit128-build \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE='-O2 -DNDEBUG'
cmake --build /tmp/bit128-build
ctest --test-dir /tmp/bit128-build --output-on-failure
```

Make 和 CMake 的测试链接独立的原版 FIPS202 作为对照；生产静态库只通过 OP
执行哈希，不包含这份测试实现。切换编译器或优化选项时使用新的 `BUILD=...`
目录，避免复用旧对象。

## 调用

包含 `api.h`，使用原有 `sig_keygen`、`sig_sign`、`sig_verify`。签名是独立的
1440 字节缓冲，不把消息拼接进签名。

| 项目 | 字节 |
|---|---:|
| 公钥 | 1264 |
| 私钥 | 1696 |
| 签名 | 1440 |

```c
#include "api.h"
#include <stdlib.h>

int example(void)
{
    unsigned char *buffers = malloc(CRYPTO_PUBLICKEYBYTES +
        CRYPTO_SECRETKEYBYTES + CRYPTO_BYTES);
    if (buffers == NULL) return -1;
    unsigned char *pk = buffers;
    unsigned char *sk = pk + CRYPTO_PUBLICKEYBYTES;
    unsigned char *sig = sk + CRYPTO_SECRETKEYBYTES;
    unsigned char message[] = "hello";
    unsigned long long pklen = 0, sklen = 0, siglen = 0;
    int rc = -1;
    if (bit_random_init() != 0) goto done;
    if (sig_keygen(pk, &pklen, sk, &sklen) != 0) goto done;
    if (sig_sign(sk, sklen, message, sizeof(message) - 1, sig, &siglen) != 0) goto done;
    rc = sig_verify(pk, pklen, sig, siglen, message, sizeof(message) - 1);
done:
    for (size_t i = 0; i < CRYPTO_PUBLICKEYBYTES + CRYPTO_SECRETKEYBYTES + CRYPTO_BYTES; ++i)
        ((volatile unsigned char *)buffers)[i] = 0;
    free(buffers);
    return rc;
}
```

生产中的每次随机请求都直接调用 `OP_trng`，包括密钥生成种子、签名随机量
和随机符号批次；没有软件 DRNG 状态，也不再导出生产侧的种子初始化接口。
`bit_random_init()` 仅是可选的随机源可用性探测，不调用也可直接生成密钥。
`randombytes.h` 提供 `bit_randombytes()`，失败时清空请求的输出缓冲。

原 DRNG 仅保存在 `platform/pc/test/reference/`，用于独立 KAT 的确定性
`OP_trng` 后端，不进入生产静态库。只有这个测试模式要求输出与旧版逐字节
相同。真实随机模式下，密钥和签名通常不同，但格式、验签及私钥互操作保持
兼容。OP 驱动的并发访问由集成方管理。
成功返回 0，失败返回负值；密钥生成或签名失败时，
非空输出长度指针置零，调用者丢弃部分输出。空消息允许传 `NULL, 0`。
输入密钥、消息和输出缓冲应互不重叠。

## 嵌入式后端

算法使用标准 `malloc/free`；每次顶层操作只分配一次工作区，拒绝重试不再
申请内存，释放前清零。调用者的公钥、私钥、签名缓冲也应放在堆或应用的
受控存储区，避免把 4400 B 的固定 IO 放到 2–3 KB 栈上。

替换 `operator_interface.h` 和 OP 实现即可接板端驱动。例如：

```sh
cmake -S PQC_BIT/platform/pc -B /tmp/bit128-board \
  -DBIT_SOFTWARE_OPS=OFF -DBUILD_TESTING=OFF \
  -DBIT_OPERATOR_INCLUDE_DIR=/path/to/board/include \
  -DBIT_OPERATOR_SOURCES=/path/to/board/op_backend.c
```

驱动需提供 `OP_hash`、`OP_hash_init/absorb/squeeze`、`OP_trng`、
`OP_ntt256_init`、`OP_ntt256`、`OP_intt256`、`OP_cwm`，算法编号沿用
仓库接口。生产哈希仅需 SHA3-256、SHAKE128/256，状态缓冲为 208 B，保证
至少 8 字节对齐。SM3 只用于确定性 KAT 后端。每次 `squeeze` 必须继续原字节流；不整块或零长度读取不能
丢掉余下字节。封装不复制活跃驱动状态，后端失败会传播到顶层。

可以用编译器宏把 `malloc/free` 接到已有的板端分配器；该分配器需满足标准 C
自然对齐要求。本模块去除了原 AVX 的 32 字节过度对齐，不要求 SIMD 分配器。

内存表、性能数据和板端核对口径见 [MEMORY_PERFORMANCE.md](MEMORY_PERFORMANCE.md)。

NTT 使用 N=256、q=7681 的完整 8 层变换，`k=0`，初始化传入两个
256 项根表，正/逆变换索引分别为 0/1。根以 R=2^32 表示；`inv_q` 位模式
为 2340676097（有符号值 -1954291199），`inv_n=1912` 是 R 缩放后的
256 逆元。输入和输出系数均在 [0,q)，`OP_cwm` 输出 a*b*R^-1 mod q。
适配层在点乘累加后乘公开常数 5569 转回普通域，多项式点乘本身由 OP 执行。
三个 256 项 int32 缓冲互不重叠，占 3072 B，放在顶层单次分配的工作区内，
按生命周期复用。算法侧没有本地 NTT 回退。

PC OP 后端仅支持上述 BiT 参数，借用生命周期覆盖所有调用的静态只读根表
（2048 B ROM），可写状态只有一个指针（本机 8 B）。板端若复制根表或
使用 DMA 暂存，应将这些额外 RAM 纳入预算。接口参数及域约定见
`platform/pc/operator_interface.h`。
