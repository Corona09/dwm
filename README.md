# Corona の dwm

Corona 配置的 dwm

## 打上了一些 patch

- alpha-systray
- alwayscenter
- autostart
- dmenumatchtop
- fullgaps
- quitprompt
- titlecolor

补丁文件见 `patched` 文件夹

## 相关脚本

`autostart` 下是自启动脚本

`scripts` 下是一些外部控制脚本

`files` 是一些相关文件

```bash
# 安装
sh sync.sh install

# 删除脚本
sh sync.sh clean
```

## MagicGrid 布局

参~~(照)~~考~~(抄)~~了 [yaocccc](https://github.com/yaocccc/dwm) 的 MagicGrid.

## 相关依赖

Arch Linux:

```bash
yay -S ttf-material-design-icons nerdfonts-jetbrains-mono \
       picom dunst \
	   feh dmenu
```
