import json
import re


obj = json.load(open("table.json", 'r'))

table = {}
for prod in obj:
    table[prod] = {}

    for key in obj[prod]:
        table[prod][key] = []
        next_prods = obj[prod][key]
        for i in range(len(next_prods)):
            table[prod][key] = next_prods[i]['rhs']

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

with open("table.out", 'w') as f:
    f.write(r)