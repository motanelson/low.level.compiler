print("\033c\033[43;30m\ngive the file name ?: ")
a=input()
b=a.replace(".ll",".list")
f1=open(a,"r")
c=f1.read()
f1.close()
d=c.split("\n")
e=""
for f in d:
    g=f.split(";")
    g=g[0].strip()
    if g!="":
        h=g.split("=")
        if len(h)>1:
            e=e+"\n"+str(h)

f1=open(b,"w")
f1.write(e)
f1.close()
