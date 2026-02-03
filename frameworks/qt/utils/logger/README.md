# Logger - Qt 日志工具类

Qt项目的简单高效日志工具类，支持日志分级、自动清理和线程安全。

## 功能特性

### 核心功能
- ✅ **日志分级**：支持 Debug、Info、Warning、Error 四个级别
- ✅ **自定义输出路径**：可配置日志文件输出目录
- ✅ **自动文件轮换**：按日期自动创建新日志文件
- ✅ **文件数量限制**：自动清理超过限制的旧日志文件
- ✅ **线程安全**：多线程环境下安全使用
- ✅ **零配置**：默认配置即可使用
- ✅ **轻量级**：仅两个文件，易于集成

### 日志级别

| 级别 | 说明 | 使用场景 |
|------|------|----------|
| Debug | 调试信息 | 开发调试、详细追踪 |
| Info | 普通信息 | 程序运行状态、关键操作 |
| Warning | 警告信息 | 潜在问题、非致命错误 |
| Error | 错误信息 | 程序错误、异常情况 |

## 快速开始

### 集成方法

将以下文件复制到您的项目：
- `logger.h`
- `logger.cpp`

在 `.pro` 文件中添加：
```qmake
HEADERS += logger.h
SOURCES += logger.cpp
```

### 基本使用

```cpp
#include "logger.h"

int main() {
    // 配置日志（可选）
    Logger::instance().setLogDirectory("logs");        // 设置输出目录
    Logger::instance().setMaxFileCount(5);             // 最多保留5个日志文件
    Logger::instance().setMinLogLevel(LogLevel::Info); // 只记录Info及以上级别
    
    // 写入日志
    Logger::debug("这是调试信息");
    Logger::info("应用程序已启动");
    Logger::warning("这是警告信息");
    Logger::error("发生错误");
    
    return 0;
}
```

## API 参考

### 配置方法

#### `void setLogDirectory(const QString &dirPath)`
设置日志输出目录。

**参数：**
- `dirPath` - 日志文件存放目录

**说明：**
- 如果目录不存在，会自动创建
- 日志文件命名格式：`app_YYYYMMDD.log`
- 默认目录：`logs`

**示例：**
```cpp
Logger::instance().setLogDirectory("C:/MyApp/logs");
Logger::instance().setLogDirectory("./application_logs");
```

#### `void setMaxFileCount(int count)`
设置最大日志文件数量。

**参数：**
- `count` - 最大保留文件数（默认：7）

**说明：**
- 超过此数量时，最旧的文件会被自动删除
- 建议设置为 3-30 之间

**示例：**
```cpp
Logger::instance().setMaxFileCount(10);  // 保留最近10天的日志
Logger::instance().setMaxFileCount(3);   // 只保留3天
```

#### `void setMinLogLevel(LogLevel level)`
设置最小日志级别。

**参数：**
- `level` - 最小记录级别（默认：Debug）

**说明：**
- 低于此级别的日志不会被记录
- 用于生产环境过滤调试信息

**示例：**
```cpp
// 开发环境：记录所有日志
Logger::instance().setMinLogLevel(LogLevel::Debug);

// 生产环境：只记录重要信息
Logger::instance().setMinLogLevel(LogLevel::Info);

// 只记录警告和错误
Logger::instance().setMinLogLevel(LogLevel::Warning);
```

### 日志输出方法

#### `static void debug(const QString &message)`
输出调试级别日志。

**示例：**
```cpp
Logger::debug("变量 x 的值为: " + QString::number(x));
Logger::debug("进入函数 processData()");
```

#### `static void info(const QString &message)`
输出信息级别日志。

**示例：**
```cpp
Logger::info("应用程序启动成功");
Logger::info("用户登录：" + username);
Logger::info("数据保存完成");
```

#### `static void warning(const QString &message)`
输出警告级别日志。

**示例：**
```cpp
Logger::warning("配置文件不存在，使用默认配置");
Logger::warning("网络连接不稳定");
Logger::warning("内存使用率达到 80%");
```

#### `static void error(const QString &message)`
输出错误级别日志。

**示例：**
```cpp
Logger::error("无法打开文件：" + filePath);
Logger::error("数据库连接失败");
Logger::error("解析 JSON 失败：" + jsonError);
```

## 使用示例

### 示例 1：基本日志记录

```cpp
#include "logger.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    Logger::info("应用程序已启动");
    Logger::debug("Qt 版本: " + QString(qVersion()));
    
    return app.exec();
}
```

### 示例 2：自定义配置

```cpp
#include "logger.h"

void initLogger() {
    // 设置日志目录
    Logger::instance().setLogDirectory("D:/MyApp/logs");
    
    // 最多保留10个日志文件
    Logger::instance().setMaxFileCount(10);
    
    // 生产环境只记录 Info 及以上级别
    #ifdef QT_NO_DEBUG
        Logger::instance().setMinLogLevel(LogLevel::Info);
    #else
        Logger::instance().setMinLogLevel(LogLevel::Debug);
    #endif
    
    Logger::info("日志系统初始化完成");
}
```

### 示例 3：错误处理

```cpp
#include "logger.h"
#include <QFile>

bool loadConfiguration(const QString &filePath) {
    QFile file(filePath);
    
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::error("无法打开配置文件: " + filePath);
        Logger::error("错误原因: " + file.errorString());
        return false;
    }
    
    Logger::info("配置文件加载成功: " + filePath);
    
    // 处理文件内容...
    
    return true;
}
```

### 示例 4：调试追踪

```cpp
#include "logger.h"

void processData(const QList<int> &data) {
    Logger::debug("processData() 开始执行");
    Logger::debug("数据数量: " + QString::number(data.size()));
    
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] < 0) {
            Logger::warning("检测到负数: " + QString::number(data[i]));
        }
        
        // 处理数据...
    }
    
    Logger::debug("processData() 执行完成");
}
```

### 示例 5：多线程使用

```cpp
#include "logger.h"
#include <QThread>

class WorkerThread : public QThread {
protected:
    void run() override {
        Logger::info("工作线程已启动");
        
        // 执行任务...
        for (int i = 0; i < 100; ++i) {
            Logger::debug("处理进度: " + QString::number(i) + "%");
            msleep(100);
        }
        
        Logger::info("工作线程已完成");
    }
};

// Logger 是线程安全的，可以在多个线程中使用
```

### 示例 6：日志格式化

```cpp
#include "logger.h"

void logUserAction(const QString &action, const QString &user, bool success) {
    QString message = QString("用户: %1 | 操作: %2 | 结果: %3")
                        .arg(user)
                        .arg(action)
                        .arg(success ? "成功" : "失败");
    
    if (success) {
        Logger::info(message);
    } else {
        Logger::error(message);
    }
}

// 使用
logUserAction("登录", "张三", true);   // [INFO] 用户: 张三 | 操作: 登录 | 结果: 成功
logUserAction("删除文件", "李四", false); // [ERROR] 用户: 李四 | 操作: 删除文件 | 结果: 失败
```

## 日志文件格式

日志文件格式：
```
[YYYY-MM-DD hh:mm:ss] [LEVEL] message
```

示例：
```
[2026-02-03 10:30:15] [INFO] 应用程序已启动
[2026-02-03 10:30:16] [DEBUG] Qt 版本: 6.5.0
[2026-02-03 10:30:20] [WARNING] 配置文件不存在，使用默认配置
[2026-02-03 10:35:42] [ERROR] 无法连接到数据库
```

## 文件管理

### 文件命名规则
- 格式：`app_YYYYMMDD.log`
- 示例：
  - `app_20260203.log` - 2026年2月3日的日志
  - `app_20260204.log` - 2026年2月4日的日志

### 自动清理机制
- 每次写入日志时，自动检查文件数量
- 如果超过 `maxFileCount` 限制，删除最旧的文件
- 按文件修改时间排序（最旧的先删除）

### 目录结构示例
```
logs/
├── app_20260128.log    (最旧，会被删除)
├── app_20260129.log
├── app_20260130.log
├── app_20260131.log
├── app_20260201.log
├── app_20260202.log
└── app_20260203.log    (今天，最新)
```

如果 `maxFileCount = 5`，则 `app_20260128.log` 和 `app_20260129.log` 会被自动删除。

## 最佳实践

### 1. 在程序启动时配置

```cpp
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    // 首先配置日志
    Logger::instance().setLogDirectory("logs");
    Logger::instance().setMaxFileCount(7);
    
    #ifdef QT_NO_DEBUG
        Logger::instance().setMinLogLevel(LogLevel::Info);
    #endif
    
    Logger::info("========== 应用程序启动 ==========");
    
    // ... 应用程序逻辑
    
    return app.exec();
}
```

### 2. 区分开发和生产环境

```cpp
void setupLogger() {
    #ifdef QT_NO_DEBUG
        // 生产环境：只记录重要信息，保留较少文件
        Logger::instance().setMinLogLevel(LogLevel::Info);
        Logger::instance().setMaxFileCount(5);
    #else
        // 开发环境：记录所有信息，保留更多文件便于调试
        Logger::instance().setMinLogLevel(LogLevel::Debug);
        Logger::instance().setMaxFileCount(30);
    #endif
}
```

### 3. 合理使用日志级别

```cpp
// ✅ 正确使用
Logger::debug("变量 x = " + QString::number(x));           // 调试信息
Logger::info("用户登录成功");                              // 重要操作
Logger::warning("配置项缺失，使用默认值");                  // 潜在问题
Logger::error("文件读取失败: " + filePath);                // 错误

// ❌ 不推荐
Logger::error("用户登录成功");                             // 这不是错误
Logger::debug("程序崩溃");                                 // 这应该是 Error
```

### 4. 避免过度日志

```cpp
// ❌ 不推荐：循环中输出大量日志
for (int i = 0; i < 1000000; ++i) {
    Logger::debug("处理第 " + QString::number(i) + " 条数据");  // 太多了！
}

// ✅ 推荐：批量记录
for (int i = 0; i < 1000000; ++i) {
    if (i % 10000 == 0) {
        Logger::debug("处理进度: " + QString::number(i));       // 每10000条记录一次
    }
}
Logger::info("共处理 1000000 条数据");
```

### 5. 包含上下文信息

```cpp
// ✅ 好的日志：包含足够上下文
Logger::error("无法打开文件: " + filePath + ", 错误: " + error);

// ❌ 不好的日志：信息不足
Logger::error("文件错误");
```

## 技术细节

### 线程安全
- 使用 `QMutex` 保护所有写操作
- 多线程环境下安全使用
- 不会出现日志交叉或文件损坏

### 性能考虑
- 每次写入都打开/关闭文件（保证数据安全）
- 适合中小型应用（每秒数千条日志）
- 如需更高性能，可考虑添加缓冲机制

### 依赖项
- Qt Core 模块
- C++11 或更高版本
- 无第三方依赖

## 常见问题

### Q: 日志文件保存在哪里？
**A:** 默认保存在程序运行目录下的 `logs` 文件夹。可以通过 `setLogDirectory()` 自定义。

### Q: 如何查看今天的日志？
**A:** 查找 `app_YYYYMMDD.log` 文件，其中 `YYYYMMDD` 是今天的日期。例如 `app_20260203.log`。

### Q: 为什么没有生成日志文件？
**A:** 检查以下几点：
1. 日志级别是否设置过高（例如设置为 Error，但只调用了 Debug）
2. 程序是否有权限在目标目录创建文件
3. 磁盘空间是否充足

### Q: 如何禁用某个级别的日志？
**A:** 使用 `setMinLogLevel()` 设置最小级别。例如：
```cpp
// 只记录 Warning 和 Error
Logger::instance().setMinLogLevel(LogLevel::Warning);
```

### Q: 日志文件太多怎么办？
**A:** 调用 `setMaxFileCount()` 减少保留的文件数量：
```cpp
Logger::instance().setMaxFileCount(3);  // 只保留3天的日志
```

### Q: 是否支持多进程？
**A:** 当前版本针对单进程设计。多进程环境建议每个进程使用不同的日志目录。

### Q: 如何自定义日志格式？
**A:** 当前版本使用固定格式。如需自定义，可以修改 `logger.cpp` 中的 `writeLog()` 方法。

## 平台支持

| 平台 | 状态 | 说明 |
|------|------|------|
| Windows | ✅ 支持 | 完全测试 |
| macOS | ✅ 支持 | 完全测试 |
| Linux | ✅ 支持 | 完全测试 |
| 其他Qt平台 | ✅ 应该支持 | 使用标准Qt API |

## 版本历史

### v1.0.0 (2026-02-03)
- 首次发布
- 支持四种日志级别（Debug、Info、Warning、Error）
- 支持自定义输出路径
- 支持文件数量限制和自动清理
- 线程安全实现

## 许可证

MIT License - 可自由用于商业和个人项目

---

## 快速参考

### 初始化
```cpp
Logger::instance().setLogDirectory("logs");
Logger::instance().setMaxFileCount(7);
Logger::instance().setMinLogLevel(LogLevel::Info);
```

### 使用
```cpp
Logger::debug("调试信息");
Logger::info("普通信息");
Logger::warning("警告信息");
Logger::error("错误信息");
```

### 集成
```qmake
HEADERS += logger.h
SOURCES += logger.cpp
```

---

**Logger - 让日志记录变得简单！** 🚀
