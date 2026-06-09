# 部署指南

## 方法一：GitHub Pages（免费，推荐）

1. 在 GitHub 创建仓库：`student-grade-manager`
2. 将本目录所有文件上传到仓库根目录
3. 进入仓库 Settings → Pages
4. Source 选择 `main` 分支，根目录 `/`
5. 保存后等1-2分钟，访问 `https://你的用户名.github.io/student-grade-manager/`

## 方法二：Vercel（免费，自动HTTPS）

1. 访问 [vercel.com](https://vercel.com) 注册
2. Import Git Repository 或直接拖拽本目录上传
3. 无需任何配置，自动部署

## 方法三：Netlify（免费）

1. 访问 [netlify.com](https://netlify.com) 注册
2. 拖拽整个目录到部署区域
3. 自动获得 `.netlify.app` 域名

## 编译 WebAssembly（可选增强）

如果你安装了 Emscripten SDK：

```bash
make          # 编译 C++ → WASM
make clean    # 清理
```

编译完成后将 `student_system.js` 和 `student_system.wasm` 放在 `index.html` 同级目录
即可自动启用 WebAssembly 加速引擎。

## 文件清单

| 文件 | 作用 |
|------|------|
| index.html | 主网页（完整功能） |
| manifest.json | PWA 清单 |
| sw.js | Service Worker（离线缓存） |
| student_system_wasm.cpp | C++ WASM 源码 |
| Makefile | Emscripten 编译脚本 |
