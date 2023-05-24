import matplotlib.pyplot as timeplt

left1 = [1, 6, 11, 16, 21, 26, 31, 36, 41, 46]

left2 = [2, 7, 12, 17, 22, 27, 32, 37, 42, 47]

left3 = [3, 8, 13, 18, 23, 28, 33, 38, 43, 48]

time1 = [20.121599, 20.680699, 20.940674, 20.815478, 20.725435, 20.770839, 20.646081, 20.103394, 19.908234, 20.504749]

time2 = [10.993226, 10.97557, 11.026062, 10.937191, 10.808721, 10.818077, 11.838386, 11.742419, 10.845097, 11.113194]

time3 = [11.040966, 10.909295, 11.136443, 10.893276, 10.843735, 10.819762, 10.826412, 10.924576, 10.796312, 10.821674]

tick_label = ["16  1  1", "12  11  11", "8  21  21", "4  31  31", "0  41  41", "-4  51  51", "-8  61  61", "-12  71  71", "-16  81  81", "-20  91  91"]

timeplt.bar(left1, time1, width = 1, color = ['red'], label = 'SCHED_OTHER')

timeplt.bar(left2, time2, tick_label = tick_label, width = 1, color = ['green'], label = 'SCHED_RR')

timeplt.bar(left3, time3, width = 1, color = ['blue'], label = 'SCHED_FIFO')

timeplt.xlabel('Scheduling priority')

timeplt.ylabel('Time')

timeplt.title('My thread timing chart!')

timeplt.legend()

timeplt.show()
