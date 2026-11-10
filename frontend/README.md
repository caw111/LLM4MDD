# LLM4MDD Studio

React、Vite 与 Tailwind 前端，对接 app/main.py 中的 FastAPI 服务。

```powershell
cd frontend
npm.cmd ci
npm.cmd run dev
```

VITE_API_BASE 可指定后端地址。默认通过 Vite 开发服务器配置访问后端。

```powershell
npm.cmd run build
```

后端安装及实验复测见仓库根目录 README.md。
