all:
	gcc -fno-stack-protector -g -o sec-agentd main.c shared/debug_op.c shared/mq_op.c os_crypto/md5/md5.c os_crypto/md5/md5_op.c os_net/os_net.c shared/sig_op.c shared/file_op.c os_regex/os_regex_strbreak.c -I . -I ./crypto -I ./client-agent
	gcc -fno-stack-protector -g -o sec-server remoted/remoted.c remoted/agentinf.c shared/debug_op.c shared/mq_op.c os_net/os_net.c shared/sig_op.c shared/file_op.c os_regex/os_regex_strbreak.c -I . -I ./client-agent
	gcc -fno-stack-protector -g -o sec-control control.c shared/debug_op.c shared/mq_op.c os_net/os_net.c shared/sig_op.c shared/file_op.c os_regex/os_regex_strbreak.c -I . -I ./client-agent
