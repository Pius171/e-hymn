import os
import json

myDict={}
values_list=[]

with open('data/hymns.json') as d:
    myDict=json.load(d)

values_list=list(myDict.values())
#print(values_list)
#print(max(values_list))
while True:
    hymn, number = input("Type in hymn and hymn number separated by a whitespace: ").split(":")
    #print(hymn,number)
    if hymn not in myDict.keys() and number not in myDict.values():
        myDict.update({hymn:int(number)})
        #print(myDict)

    else:
        print("already exist")
    
    with open('data/hymns.json', 'w') as fp:
        json.dump(myDict, fp,indent=1)
        print(json.dumps(myDict,indent=1))
        
        
        