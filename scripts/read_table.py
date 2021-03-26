import json
import re


obj = json.load(open("table.json", 'r'))

table = {}
for prod in obj:
    table[prod] = {}

    for key in obj[prod]:
        table[prod][key] = []
        # print(key, obj[prod][key])
        next_prods = obj[prod][key]
        for i in range(len(next_prods)):
            table[prod][key] = next_prods[i]['rhs']

# print(json.dumps([1], cls=JSONEncoder))

# json.dump(table, open("tb.json", 'w'), indent=4)
# q = q.replace('[', '{')
# q = q.replace(']', '}')
# q = re.sub(r"([^ ]*): (.*),", r"{\1, \2},", q)

# while True:
#     try:
#         q = re.sub(r"([^ ]*): {((.*\n)*)}", r"{{\1, \2}}", q)
#         q.index(':')
#     except ValueError:
#         break
# # q = ''.join(q.split())



r = "{"
for prod in table:
    r += "{\"" + prod + "\",{"
    for key in table[prod]:
        r += "{\"" + key + "\"," + str(table[prod][key]) + "},"
    if r[-1] == ',':
        r = r[:-1]
    r += "}},"
if r[-1] == ',':
    r = r[:-1]
r += "}"
r = r.replace('[', '{')
r = r.replace(']', '}')
r = r.replace("'", "\"")

with open("tb.json", 'w') as f:
    f.write(r)