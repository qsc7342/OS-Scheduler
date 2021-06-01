import plotly.figure_factory as ff
import re
d = []
with open("log.txt") as f:
    for line in f:
       (t, s, e, r) = line.split()
       a = re.findall("\d", t) 
       print(a)
       cur = 1
       val = 0
       for x in reversed(a):
           x = int(x)
           val = val + cur * x
           cur = cur * 10
       newdict = dict(Task=val, Start=s, Finish=e, Resource=r)
       d.append(newdict)

sorted_d = sorted(d, key=(lambda x: x['Task']))
fig = ff.create_gantt(sorted_d, index_col='Resource', show_colorbar=True, group_tasks=True, bar_width = 0.1, height = 300, width = 450)
fig.update_layout(xaxis_type='linear')
fig.write_html('gantt_chart.html', auto_open=True)