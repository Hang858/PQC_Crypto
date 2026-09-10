# PolarLAC KEM 接口映射

项目中的接口：

```c
crypto_kem_keypair(pk, sk);
crypto_kem_enc(c, k1, pk);
crypto_kem_dec(k2, c, sk);
```

对应关系如下：

| 操作 | 本项目 PolarLAC | openHiTLS/PQCP |
|---|---|---|
| 密钥生成 | `crypto_kem_keypair(pk, sk)` | 设置安全等级后调用 `CRYPT_EAL_PkeyGen(ctx)`，再通过 `CRYPT_EAL_PkeyGetPubEx()` 和 `CRYPT_EAL_PkeyGetPrvEx()` 取出 `pk`、`sk` |
| 封装 | `crypto_kem_enc(c, k1, pk)` | `CRYPT_EAL_PkeyEncapsInit(ctx, NULL)`，然后 `CRYPT_EAL_PkeyEncaps(ctx, c, &cLen, k1, &kLen)` |
| 解封装 | `crypto_kem_dec(k2, c, sk)` | `CRYPT_EAL_PkeyDecapsInit(ctx, NULL)`，然后 `CRYPT_EAL_PkeyDecaps(ctx, c, cLen, k2, &kLen)` |

其中：

- `c`：密文。
- `k1`、`k2`：32 字节共享密钥。
- `pk`：封装使用的公钥。
- `sk`：解封装使用的私钥。
- `ctx`：PQCP 上下文；调用封装前必须设置公钥，调用解封装前必须设置私钥和对应的 PolarLAC 安全等级。


它们内部最终调用上述 `crypto_kem_enc()` 和 `crypto_kem_dec()`。
