compile : phil_strict_order phil_bowl_strict_order phil_sem phil_sem_bowl

phil_strict_order : phil_strict_order.c
	gcc -o phil_strict_order phil_strict_order.c -lpthread

phil_bowl_strict_order : phil_bowl_strict_order.c
	gcc -o phil_bowl_strict_order phil_bowl_strict_order.c -lpthread

phil_sem : phil_sem.c
	gcc -o phil_sem phil_sem.c -lpthread

phil_sem_bowl : phil_sem_bowl.c
	gcc -o phil_sem_bowl phil_sem_bowl.c -lpthread
