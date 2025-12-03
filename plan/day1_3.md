# ⚔️ 深圳就业突击：前 72 小时特种兵执行方案

**目标**：从 Windows 图形界面编程，彻底转变为 Linux 命令行工程化开发。
**口号**：No Mouse, No GUI.（尽量少用鼠标，没有图形界面）

---

## 📅 Day 1：环境搭建与 Linux 生存指南
**核心目标**：在你的 Windows 电脑上“长出”一个 Linux 系统，并学会如何在黑框框里活下来。

### 🕘 上午 09:00 - 12:00：环境配置 (WSL2 + VS Code)
这是现代 C++ 开发的标准姿势，比虚拟机轻量，比双系统方便。

1. **动作 1：一键安装 WSL2**
   - **步骤**：
     1. 按 `Win + X`，选择 **“终端 (管理员)”** 或 **PowerShell (管理员)**。
     2. 输入命令：`wsl --install`
     3. **重启电脑**。重启后会自动弹出 Ubuntu 安装窗口，设置用户名（建议全小写，如 `admin`）和密码。
   - **文章参考**：[微软官方 WSL 安装教程](https://learn.microsoft.com/zh-cn/windows/wsl/install)

2. **动作 2：配置 VS Code 神器**
   - **步骤**：
     1. 在 Windows 打开 VS Code。
     2. 点击左侧扩展商店，搜索并安装插件：**WSL** (出品方 Microsoft)。
     3. 回到 Ubuntu 终端，输入 `code .`（注意有个点）。
     4. 神奇的事情发生了：VS Code 会打开，但它现在是运行在 Linux 里的！
   - **视频参考**：B 站搜索 **“VSCode 配置 WSL C++”**，推荐 UP 主“**极客湾Geekerwan**”或“**CodeSheep**”的相关环境搭建视频（看前 10 分钟即可）。

### 🕑 下午 14:00 - 17:00：Linux 命令行特训
**禁止使用文件管理器**。所有的文件操作必须用键盘完成。

1. **动作 1：刷完 Linux 基础命令**
   - **必练命令清单**（每个敲 5 遍以上）：
     - `pwd` (我在哪), `ls -al` (看文件), `cd ..` (回去)
     - `mkdir project` (建文件夹), `touch main.cpp` (建文件)
     - `cp` (复制), `mv` (移动/改名), `rm -rf` (删除，**慎用**)
     - `cat` (看文件内容), `grep "hello" main.cpp` (查找内容)
   - **练习文章**：[Linux 命令大全 (菜鸟教程)](https://www.runoob.com/linux/linux-command-manual.html) —— 当作字典查。

2. **动作 2：GCC 编译初体验**
   - **步骤**：
     1. 写一个 `hello.cpp`。
     2. 不用 VS Code 的运行按钮。在终端输入：`g++ hello.cpp -o hello`。
     3. 运行它：`./hello`。

---

## 📅 Day 2：现代化构建 (CMake) 与协作 (Git)
**核心目标**：告别 `g++ main.cpp` 这种手搓命令，学会大厂标准的构建方式。

### 🕘 上午 09:00 - 12:00：CMake 入门 (重中之重)
大厂的 C++ 项目不用 Visual Studio 的 `.sln`，而是用 `CMakeLists.txt`。

1. **学习资料 (必看)**：
   - **GitHub 教程**：[Modern CMake 简体中文版](https://github.com/Modern-CMake-CN/Modern-CMake-zh_CN) —— **只看“介绍”和“基础”章节**。
   - **B 站视频**：搜索 **“CMake 保姆级教程”**（推荐 UP 主：**空念** 或 **Subingwen**）。

2. **实操任务**：
   - 创建一个文件夹 `cmake_demo`。
   - 新建 `main.cpp`, `calc.cpp`, `calc.h`。
   - **目标**：手写一个 `CMakeLists.txt`，把它们编译成可执行文件 `app`。
   - **关键代码**：
     ```cmake
     cmake_minimum_required(VERSION 3.10)
     project(MyProject)
     add_executable(app main.cpp calc.cpp)
     ```
   - **构建流程**：`mkdir build && cd build && cmake .. && make`

### 🕑 下午 14:00 - 17:00：Git 版本控制
如果你以后进大厂，不懂 Git 会被 Mentor 骂死。

1. **学习资料**：
   - **互动游戏**：[Learn Git Branching](https://learngitbranching.js.org/?locale=zh_CN) —— **强烈推荐**，像玩游戏一样学 Git，通关前两章即可。
   - **文章**：[廖雪峰 Git 教程](https://www.liaoxuefeng.com/wiki/896043488029600) —— 查阅用。

2. **实操任务**：
   - 去 GitHub 建一个仓库叫 `KV-Store-Learning`。
   - 配置 SSH Key（搜教程：GitHub 配置 SSH）。
   - 把上午写的 CMake 代码 `push` 上去。

---

## 📅 Day 3：调试 (GDB) 与 综合实战
**核心目标**：学会不靠 `cout` 打印来调试程序。

### 🕘 上午 09:00 - 12:00：GDB 调试艺术
ACMer 喜欢用脑子模拟代码，但工程代码太复杂，必须用 GDB。

1. **视频学习**：
   - **B 站视频**：搜索 **“GDB 调试入门”**。推荐看 30 分钟左右的实操视频。
   - **重点技能**：
     - `g++ -g main.cpp -o app` (加 -g 才能调试)
     - `gdb app` (启动)
     - `b main` (打断点)
     - `r` (运行)
     - `n` (下一步), `s` (进入函数)
     - `p variable_name` (打印变量)
     - `bt` (查看堆栈 —— **程序崩溃时必用**)

2. **实战作业**：
   - 写一个会产生 `Segmentation Fault` (段错误) 的代码（比如访问空指针）。
   - 用 GDB 运行它，当它崩溃时，输入 `bt`，看它死在哪一行。

### 🕑 下午 14:00 - 17:00：综合演练 (Pre-Project)
为明天的 KV 存储项目热身。

1. **任务**：写一个简单的 **“C++ 读取文件并排序”** 的小程序。
   - **需求**：
     1. 用 `fstream` 读取一个 txt 文件。
     2. 用 `std::sort` 排序。
     3. 输出到新文件。
   - **要求**：
     1. 使用 **Linux** 环境。
     2. 使用 **CMake** 构建。
     3. 使用 **Git** 提交代码。

---

## ✅ Day 1-3 验收清单
如果在第三天晚上，你能做到以下几点，说明你已经准备好进入大厂开发模式了：
- [ ] 可以在 Windows 下通过 VS Code 流畅编写运行 Linux C++ 代码。
- [ ] 能够不看教程写出一个简单的 `CMakeLists.txt`。
- [ ] 知道 `git add`, `git commit`, `git push` 是什么意思。
- [ ] 遇到程序崩溃（SegFault），第一反应是用 GDB 看堆栈 (`bt`)，而不是瞎猜。