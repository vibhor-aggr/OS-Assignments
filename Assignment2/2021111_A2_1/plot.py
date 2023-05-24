import matplotlib.pyplot as timeplt

left1 = [1, 6, 11, 16, 21, 26, 31, 36, 41, 46]

left2 = [2, 7, 12, 17, 22, 27, 32, 37, 42, 47]

left3 = [3, 8, 13, 18, 23, 28, 33, 38, 43, 48]

time1 = [20.286328, 20.257391, 20.628720, 20.605772, 19.970495, 20.402292, 19.739670, 20.224801, 20.081331, 20.514693]

time2 = [10.825264, 10.627781, 11.301925, 10.740311, 11.328692, 11.123756, 10.993791, 10.688421, 11.076977, 10.703952]

time3 = [10.617561, 10.647292, 11.234017, 10.774034, 10.636083, 10.665710, 10.970042, 10.681209, 10.433382, 10.692229]

tick_label = ["16  1  1", "12  11  11", "8  21  21", "4  31  31", "0  41  41", "-4  51  51", "-8  61  61", "-12  71  71", "-16  81  81", "-20  91  91"]

timeplt.bar(left1, time1, width = 1, color = ['red'], label = ['SCHED_OTHER'])

timeplt.bar(left2, time2, tick_label = tick_label, width = 1, color = ['green'], label = ['SCHED_RR'])

timeplt.bar(left3, time3, width = 1, color = ['blue'], label = ['SCHED_FIFO'])

timeplt.xlabel('Scheduling priority')

timeplt.ylabel('Time')

timeplt.title('My thread timing chart!')

timeplt.legend()

timeplt.show()