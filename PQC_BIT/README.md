# BiT-128 普通版

提供 128 位安全等级的普通密钥生成、签名和验签，不包含预计算版本。
哈希/SHAKE、随机数、正逆 NTT 和点乘均通过 OP 接口，PC 提供软件后端。

- `include/`：公共 API，入口为 `api.h`。
- `core/common/`：算法实现。
- `platform/pc/`：构建、OP 接口、软件后端和测试。

公共接口为 `sig_keygen`、`sig_sign`、`sig_verify`；公钥/私钥/签名大小分别为
1264/1696/1440 字节。每次顶层操作仅分配一次工作区，释放前清零。

在仓库根目录执行：

```sh
make -C PQC_BIT/platform/pc
make -C PQC_BIT/platform/pc check memory stack kat
```

默认构建 O2，静态库输出为 `platform/pc/build/libpqc_bit.a`。
O3、板端接入和原版对照测试见 [接入说明](INTEGRATION.md)，
内存预算与性能数据见 [测试报告](MEMORY_PERFORMANCE.md)。
