import json
import re


obj = json.load(open("transition.json", 'r'))

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

with open("transition.out", 'w') as f:
    f.write(r)


obj = json.load(open("t_grammar.json", 'r'))

grammar = {}
for r in obj:
    name = r["lhs"]
    if len(r["rhs"]) > 0:
        if name in grammar:
            grammar[name].append(r["rhs"])
        else:
            grammar[name] = [r["rhs"]]

r = "{"
for key in grammar:
    r += "{\"" + key + "\"," + str(grammar[key]) + "},"
if r[-1] == ',':
    r = r[:-1]
r += "}"
r = r.replace('[', '{')
r = r.replace(']', '}')
r = r.replace("'", "\"")
print(r)

with open("t_grammar.out", 'w') as f:
    f.write(r)