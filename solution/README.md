# solution

这是助教的实现版本，对应于 `src/surakarta_ta/surakarta_rule_manager_imp.cpp`

查看他需要解密密码，你可以在 issue 中向作者索要

## 如何查看/使用

1. 安装 GnuPG (GPG)：`sudo apt install gnupg`
2. 解密到对应的目录：`gpg -o src/surakarta_ta/surakarta_rule_manager_imp.cpp -d solution/surakarta_rule_manager_imp.cpp.gpg`
3. 重新构建项目：你需要在先创建 CMake 构建时指定 `-DENV=DEV`，否则它会直接使用预编译的动态链接库，例如 `cmake -DENV=DEV ..`
4. `make -j`
5. 可选：`make install`，这会将动态链接库拷贝到 `surakarta_ta/lib` 下
