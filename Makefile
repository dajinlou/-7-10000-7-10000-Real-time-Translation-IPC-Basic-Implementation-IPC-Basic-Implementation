CC=g++
CFLAGS=-Wall -g
LIRRARY = -lrt -pthread
BINDIR=./bin

# 管道
target1=$(BINDIR)/pipe_test
src1=pipe_test.cpp
# 管道2.0
target2=$(BINDIR)/pipe_test2
src2=pipe_test2.cpp

# 命名管道
target3=$(BINDIR)/fifo_read
src3=fifo_read.cpp
target4=$(BINDIR)/fifo_write
src4=fifo_write.cpp

# 内存映射
target5=$(BINDIR)/mmap_test
src5=mmap_test.cpp

#本地套接字
#服务端
target6=$(BINDIR)/local_socket
src6=local_socket.cpp
#客户端
target7=$(BINDIR)/local_socket_client
src7=local_socket_client.cpp

#System V 共享内存
target8=$(BINDIR)/system_v_shm_test
src8=system_v_shm_test.cpp

#SYstem V 消息队列
target9=$(BINDIR)/system_v_msg_test
src9=system_v_msg_test.cpp

#SYstem V 信号量
target10=$(BINDIR)/system_v_sem_test
src10=system_v_sem_test.cpp

#SYstem V 信号量 + 共享内存
target11=$(BINDIR)/system_v_shm_syn_test
src11=system_v_shm_syn_test.cpp

#POSIX 消息队列
target12=$(BINDIR)/posix_mq_test
src12=posix_mq_test.cpp

target13=$(BINDIR)/posix_mq_notify_test
src13=posix_mq_notify_test.cpp


#POSIX 信号量
target14=$(BINDIR)/posix_sem_test
src14=posix_sem_test.cpp

#POSIX 共享内存
target15=$(BINDIR)/posix_shm_test
src15=posix_shm_test.cpp

#POSIX 共享内存+信号量 实现同步
target16=$(BINDIR)/posix_shm_sem_test
src16=posix_shm_sem_test.cpp


all:${target1} ${target2} ${target3} ${target4} ${target5} ${target6} ${target7} ${target8} \
	${target9} ${target10} ${target11} ${target12} ${target13} ${target14} ${target15} ${target16}
# 管道
$(target1):$(src1)
	$(CC) $(src1) -o $(target1) $(CFLAGS)
# 管道2.0
$(target2):$(src2)
	$(CC) $(src2) -o $(target2) $(CFLAGS)

# 命名管道
$(target3):$(src3)
	$(CC) $(src3) -o $(target3) $(CFLAGS)
$(target4):$(src4)
	$(CC) $(src4) -o $(target4) $(CFLAGS)

# 内存映射
$(target5):$(src5)
	$(CC) $(src5) -o $(target5) $(CFLAGS)

# 本地套接字
$(target6):$(src6)
	$(CC) $(src6) -o $(target6) $(CFLAGS)
$(target7):$(src7)
	$(CC) $(src7) -o $(target7) $(CFLAGS)

# System V 共享内存
$(target8):$(src8)
	$(CC) $(src8) -o $(target8) $(CFLAGS)

#SYstem V 消息队列
$(target9):$(src9)
	$(CC) $(src9) -o $(target9) $(CFLAGS)

#SYstem V 信号量
$(target10):$(src10)
	$(CC) $(src10) -o $(target10) $(CFLAGS)

#SYstem V 信号量+共享内存
$(target11):$(src11)
	$(CC) $(src11) -o $(target11) $(CFLAGS)

#POSIX 消息队列
$(target12):$(src12)
	$(CC) $(src12) -o $(target12) $(CFLAGS) $(LIRRARY)

#POSIX 消息队列 + notify
$(target13):$(src13)
	$(CC) $(src13) -o $(target13) $(CFLAGS) $(LIRRARY)


#POSIX 信号量
$(target14):$(src14)
	$(CC) $(src14) -o $(target14) $(CFLAGS) $(LIRRARY)

#POSIX 共享内存
$(target15):$(src15)
	$(CC) $(src15) -o $(target15) $(CFLAGS) $(LIRRARY)

#POSIX 共享内存+信号量 实现同步
$(target16):$(src16)
	$(CC) $(src16) -o $(target16) $(CFLAGS) $(LIRRARY)



# 清理生成的文件
clean:
	rm -f $(BINDIR)/*
	@echo "Clean up done!"