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
- pertag
- dwmblocks

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

<center>
	<img
		src="https://corona-oss.oss-cn-qingdao.aliyuncs.com//img/2023-01-20_14-24-45.L88o4Jbz4.dwm-layout-grid.png"
	/>
</center>

## Horizontal Tile 布局

对 tile 布局的魔改, 将 master 窗口水平放置于上方.

<center>
	<img
		src="https://corona-oss.oss-cn-qingdao.aliyuncs.com//img/2023-01-20_14-26-00.IZ8BMiTgO.dwm-layout-htile.png"
	/>
</center>

## 相关依赖

Arch Linux:

```bash
yay -S ttf-material-design-icons nerdfonts-jetbrains-mono \
       picom dunst \
	   feh dmenu
```
