# 🚀 30天 C++ 后端/游戏开发突击作战计划 (V2.0)

**目标**：从 ACM 铜奖选手转型为具备工程能力的 C++ 工程师
**战场**：2026 届春招（目标城市：深圳）
**核心武器**：基于跳表的高性能键值存储引擎 (High-Performance KV Store)

---

## 📅 第一阶段：环境与地基 (Day 1 - 3) [已完成 ✅]
**状态**：戒断图形界面，适应 Linux 命令行。

- [x] **Day 1**: 纯 Linux 环境生存 (WSL2 + VS Code)
- [x] **Day 2**: 现代化构建 (CMake) & 协作 (Git)
- [x] **Day 3**: 调试艺术 (GDB) & 工程化演练 (SortDemo)

---

## 💻 第二阶段：核心引擎与持久化 (Day 4 - 10) [已完成 ✅]
**项目**：基于跳表 (SkipList) 的高性能键值存储引擎

- [x] **Day 4**: 理解跳表原理，搭建项目骨架 (Template Class)。
- [x] **Day 5**: 实现核心逻辑：Insert (插入) 与 Search (查询)。
- [x] **Day 6**: 实现内存管理：Erase (删除) 与 Destructor (析构)。
- [x] **Day 7**: 代码重构与工程化 (Random Engine 封装, 路径优化)。
- [x] **Day 8**: 文件格式设计 (二进制协议 + 泛型序列化)。
- [x] **Day 9**: 实现数据落盘 `dump_file` (Snapshot)。
- [x] **Day 10**: 实现数据恢复 `load_file` (Recovery)。

---

## 🌐 第三阶段：高性能网络层 (Day 11 - 15) —— **当前战役** 🚩
**目标**：实现 Client-Server 架构，基于 Epoll 的 Reactor 模型（毕设重点）。

### Day 11: Socket 编程基础 (Blocking IO)
- [ ] **理论**: 理解 TCP 三次握手、File Descriptor。
- [ ] **实战**: 编写 `simple_server.cpp`，实现最基础的 `socket` -> `bind` -> `listen` -> `accept` 流程，跑通 `telnet` 连接。

### Day 12: IO 多路复用与 Epoll (The C10K Problem)
- [ ] **理论**: 阻塞 IO vs 非阻塞 IO vs 多路复用。理解 `select`, `poll`, `epoll` 的区别。
- [ ] **实战**: 引入 `epoll_create`, `epoll_ctl`, `epoll_wait`，改造 Server，使其能同时维持多个空闲连接而不阻塞。

### Day 13: Reactor 模型封装
- [ ] **架构**: 将杂乱的 socket API 封装成 C++ 类 (`Epoll`, `Socket`, `InetAddress`)。
- [ ] **实战**: 实现一个基于事件驱动的 Event Loop（事件循环），将“连接事件”和“读写事件”分离。

### Day 14: 应用层协议与命令解析 (Protocol Parser)
- [ ] **设计**: 定义简单的文本协议（如 `SET key val`, `GET key`）。
- [ ] **实战**: 编写解析器函数，处理粘包问题（简单处理），将接收到的字符串解析为操作指令。

### Day 15: 系统联调 (Integration)
- [ ] **实战**: 将 **网络层** 与 **存储引擎(SkipList)** 对接。
- [ ] **验证**: 启动 Server，使用多个 Client 终端同时发送 `SET` 和 `GET` 命令，验证数据一致性。

---

## ⚡ 第四阶段：并发优化与性能压测 (Day 16 - 20) —— **毕设亮点** ✨
**目标**：解决多线程竞争问题，并产出性能测试报告。

### Day 16: 线程池 (Thread Pool)
- [ ] **痛点**: 单线程处理请求太慢，每来一个请求 `new` 一个线程太重。
- [ ] **实战**: 手写一个固定大小的线程池（基于 `std::thread`, `std::mutex`, `std::condition_variable`），将计算任务（KV操作）抛给工作线程。

### Day 17: 读写锁优化 (RWLock)
- [ ] **优化**: KV 存储通常“读多写少”。`std::mutex` 是互斥锁，读读也互斥，性能差。
- [ ] **实战**: 引入 C++17 `std::shared_mutex` (读写锁)。
    - `search`: 使用 `shared_lock` (共享读，不阻塞其他读)。
    - `insert/erase`: 使用 `unique_lock` (独占写)。

### Day 18: 压力测试工具 (Benchmark Tool)
- [ ] **任务**: 编写 `stress_test.cpp`。
- [ ] **指标**: QPS (Queries Per Second) 和 Latency (延迟)。
- [ ] **对比**: 设置对照组 —— **你的 SkipList VS C++ STL `std::map`** (红黑树)。

### Day 19: 数据分析与调优
- [ ] **实战**: 在不同线程数（1, 4, 8, 16）下跑分，记录数据。
- [ ] **产出**: 生成 Excel 图表（这将直接贴进你的毕设论文和简历）。

### Day 20: 项目收尾与文档
- [ ] **文档**: 编写详细的 `README.md` (怎么编译、怎么跑、性能数据)。
- [ ] **清理**: 统一代码风格 (Google Style)，添加注释，清理无用 Debug 输出。

---

## 📚 第五阶段：八股文与简历突击 (Day 21 - 27)
**战略**：用项目反推八股文，实现“降维打击”。

- [ ] **Day 21-23**: **操作系统 & 网络八股**
    - 重点：Epoll 原理 (红黑树+双向链表)、零拷贝 (Zero Copy)、TCP 状态机 (`TIME_WAIT`)。
- [ ] **Day 24-25**: **C++ 硬核特性**
    - 重点：智能指针 (`shared_ptr` 实现原理)、右值引用与移动语义、虚函数表。
- [ ] **Day 26-27**: **简历终极打磨**
    - 模拟面试：如何用 3 分钟讲清楚你的 KV Store 架构？

---

## ⚔️ 第六阶段：实战演练 (Day 28 - 30)
**战略**：调整心态，准备把子弹打出去。

- [ ] **Day 28**: 模拟面试 (手撕代码 + 场景设计)。
- [ ] **Day 29**: 搜集内推码与岗位情报。
- [ ] **Day 30**: 正式出击投递。