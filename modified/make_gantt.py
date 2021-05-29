import plotly.figure_factory as ff
d = []
with open("log.txt") as f:
    for line in f:
       (t, s, e, r) = line.split()
       newdict = dict(Task=t, Start=s, Finish=e, Resource=r)
       d.append(newdict)

sorted_d = sorted(d, key=(lambda x: x['Task']))
fig = ff.create_gantt(sorted_d, index_col='Resource', show_colorbar=True, group_tasks=True)
fig.update_layout(xaxis_type='linear')
fig.write_html('gantt_chart.html', auto_open=True)