compile : fifo_sender fifo_receiver socket_sender socket_receiver shm_sender shm_receiver

fifo_sender : fifo_sender.c
	gcc -o fifo_sender fifo_sender.c

fifo_receiver : fifo_receiver.c
	gcc -o fifo_receiver fifo_receiver.c

socket_sender : socket_sender.c
	gcc -o socket_sender socket_sender.c

socket_receiver : socket_receiver.c
	gcc -o socket_receiver socket_receiver.c

shm_sender : shm_sender.c
	gcc -o shm_sender shm_sender.c -lrt

shm_receiver : shm_receiver.c
	gcc -o shm_receiver shm_receiver.c -lrt
