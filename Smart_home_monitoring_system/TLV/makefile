# 找到所有.cc源文件
SRCS := $(wildcard *.cc)

# 排除TLV.cc，只将其他.cc文件作为可执行程序的源文件
EXEC_SRCS := $(filter-out TLV.cc, $(SRCS))

# 为每个可执行源文件生成对应的程序名（去掉.cc后缀）
EXES := $(patsubst %.cc, %, $(EXEC_SRCS))

# 编译器和编译选项
CC := g++
FLAGS := -Wall -g

# 定义TLV相关文件（核心依赖）
TLV_SRC := TLV.cc
TLV_OBJ := $(patsubst %.cc, %.o, $(TLV_SRC))

# 明确指定依赖TLV的程序（客户端和服务端）
TLV_DEP_PROGRAMS := client_EPOLLET server_EPOLLET

# 其他不依赖TLV的程序（如果有的话）
OTHER_PROGRAMS := $(filter-out $(TLV_DEP_PROGRAMS), $(EXES))

# 默认目标：编译所有可执行程序
all: $(TLV_DEP_PROGRAMS) $(OTHER_PROGRAMS)

# 规则：编译依赖TLV的程序（链接TLV.o）
$(TLV_DEP_PROGRAMS): %: %.cc $(TLV_OBJ)
	$(CC) -o $@ $^ $(FLAGS)

# 规则：编译不依赖TLV的程序
$(OTHER_PROGRAMS): %: %.cc
	$(CC) -o $@ $< $(FLAGS)

# 编译TLV的目标文件（仅生成.o，不生成可执行程序）
$(TLV_OBJ): $(TLV_SRC)
	$(CC) -c $< -o $@ $(FLAGS)

# 清理所有生成的文件
clean:
	rm -rf $(EXES) *.o

# 调试用：查看变量
echo:
	@echo "所有源文件: $(SRCS)"
	@echo "可执行程序源文件: $(EXEC_SRCS)"
	@echo "可执行程序: $(EXES)"
	@echo "依赖TLV的程序: $(TLV_DEP_PROGRAMS)"

