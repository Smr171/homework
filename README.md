# 学生成绩管理系统 — 基于 C++ WebAssembly 的云端成绩管理平台

## 📋 摘要

本系统针对高校成绩管理场景，设计并实现了一套**双端分离**的成绩管理系统。**教师端**支持班级管理、成绩录入/编辑/删除、多维度排序、可视化分析、评语反馈、数据导出；**学生端**支持个人成绩查询、GPA 查看、教师评语阅读。核心算法采用 **C++17 标准库**实现（排序、统计、分布计算），通过 **Emscripten** 编译为 **WebAssembly** 在浏览器端运行；数据存储采用 **Firebase Firestore** 云端数据库，实现跨设备数据同步；前端基于纯 **HTML/CSS/JavaScript** 构建，集成 **ECharts** 可视化图表，响应式布局适配桌面与移动端。

**关键词**：成绩管理系统；WebAssembly；Firebase；C++；ECharts；GPA

## Abstract

This project implements a dual-role student grade management system. The teacher portal supports class management, grade CRUD, multi-dimensional sorting, visual analytics, feedback comments, and data export. The student portal provides personal grade inquiry, GPA viewing, and teacher comment reading. Core algorithms are implemented in C++17 and compiled to WebAssembly via Emscripten. Cloud data storage uses Firebase Firestore for cross-device synchronization. The frontend is built with vanilla HTML/CSS/JavaScript, integrated with ECharts for visualization, featuring responsive design.

**Keywords**: Grade Management System; WebAssembly; Firebase; C++; ECharts; GPA

---

## 🏗 系统架构

```
┌─────────────────────────────────────────────────┐
│                    浏览器                        │
│  ┌──────────┐  ┌──────────┐  ┌──────────────┐  │
│  │ 教师端 UI │  │ 学生端 UI │  │ C++ → WASM   │  │
│  └────┬─────┘  └────┬─────┘  │ (核心算法)    │  │
│       │             │         └──────┬───────┘  │
│       └──────┬──────┘                │           │
│              │                       │           │
│     ┌────────▼────────┐    ┌────────▼────────┐  │
│     │   Firebase SDK   │    │  JavaScript 引擎 │  │
│     │  (Auth+Firestore)│    │    (降级方案)    │  │
│     └────────┬─────────┘    └────────────────┘  │
└──────────────┼──────────────────────────────────┘
               │
     ┌─────────▼─────────┐
     │   Google Cloud    │
     │  Firebase 后端     │
     └───────────────────┘
```

## 🛠 技术栈

| 层级 | 技术 | 说明 |
|------|------|------|
| 核心算法 | C++17 | std::accumulate, std::sort, Lambda |
| 浏览器运行 | WebAssembly (Emscripten) | C++ 编译为 .wasm |
| 前端框架 | 原生 HTML/CSS/JS | 零依赖，响应式布局 |
| 可视化 | ECharts 5.5 | 柱状图、雷达图、折线图 |
| 云端后端 | Firebase Auth + Firestore | 用户认证 + NoSQL 数据库 |
| 代码高亮 | highlight.js | C++ 源码展示 |
| 部署 | GitHub Pages + Actions | CI/CD 自动部署 |
| 降级方案 | 纯 JavaScript 引擎 | WASM 不可用时自动切换 |

## 📦 功能模块

### 教师端
- ✏️ 学生信息录入（自选课程，支持勾选）
- 📊 成绩总表（排序、编辑、删除、评语）
- 📈 可视化报表（柱状图、雷达图、趋势折线图）
- 📐 各科统计分析（总分、平均分、GPA）
- 📉 成绩分布统计（6段分布 + 正态曲线）
- 📥 导出 CSV（Excel 可打开）
- 💾 数据备份/恢复（JSON 格式）
- ✅ 学生注册审核
- 👥 用户管理
- 📤 批量导入 CSV
- 🎓 GPA 计算（学分绩点制）

### 学生端
- 📋 个人成绩查询
- 🏆 总分排名
- 💬 教师评语查看
- 📊 GPA 绩点显示
- 🔒 只读模式，数据安全

### 系统特性
- 🌙 深色/亮色模式切换
- 📱 响应式布局（桌面/平板/手机）
- ☄️ 星空粒子登录背景
- 🔐 登录锁定（3次错误锁定30秒）
- ⚡ WASM 加速 / JS 降级双引擎

## 🧮 GPA 计算规则

| 百分制 | 绩点 |
|--------|------|
| 90-100 | 4.0 |
| 85-89  | 3.7 |
| 82-84  | 3.3 |
| 78-81  | 3.0 |
| 75-77  | 2.7 |
| 72-74  | 2.3 |
| 68-71  | 2.0 |
| 64-67  | 1.5 |
| 60-63  | 1.0 |
| <60    | 0   |

**GPA = Σ(绩点 × 学分) / Σ学分**

## 📁 项目结构

```
├── index.html              # 主页面（教师端+学生端+源码+说明）
├── manifest.json           # PWA 配置
├── sw.js                   # Service Worker 离线缓存
├── student_system_wasm.cpp # C++ WebAssembly 源码
├── Makefile                # Emscripten 编译脚本
├── README.md               # 本文件
├── DEPLOY.md               # 部署指南
└── 新建文件夹/
    └── 部分代码             # 原始 C++ 控制台代码
```

## 🚀 部署

### 快速部署（GitHub Pages）

1. Fork 本仓库
2. Settings → Pages → Source: GitHub Actions
3. 访问 `https://你的用户名.github.io/仓库名/`

### Firebase 配置

1. 在 [Firebase Console](https://console.firebase.google.com/) 创建项目
2. 启用 Authentication（邮箱/密码）和 Firestore
3. 复制配置到 `index.html` 中的 `firebaseConfig`
4. 部署 Firestore 安全规则

## 🧪 测试

```bash
npm test  # 运行 Jest 单元测试
```

测试覆盖：引擎核心算法（computeOneStudent、makeRankList、GPA计算、排序）

## 📝 创新点

1. **C++ WebAssembly 混合架构**：核心算法用 C++ 编写编译为 WASM，前端用 JS 做 UI 层，兼顾性能与开发效率
2. **双引擎容错**：WASM 不可用时自动降级为纯 JS 引擎，保证全平台兼容
3. **双端分离设计**：教师端与学生端权限严格分离，注册审核机制
4. **GPA 学分绩点制**：支持自定义学分，自动计算绩点与 GPA
5. **零框架前端**：纯原生 HTML/CSS/JS，无 React/Vue 依赖，极致轻量
