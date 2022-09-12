import os
import json

myDict = {}
values_list = []

with open('data/hymns.json') as d:
    myDict = json.load(d)

values_list = list(myDict.values())
# print(values_list)
# print(max(values_list))
number = int(input("Start count from: "))
while True:

    hymn = input("Type in hymn: ")
    # print(hymn,number)
    if hymn not in myDict.keys() and number not in myDict.values():
        myDict.update({hymn.title(): number})
        number += 1

    else:
        print("already exist")

    with open('data/hymns.json', 'w') as fp:
        json.dump(myDict, fp, indent=1)
        print(json.dumps(myDict, indent=1))
