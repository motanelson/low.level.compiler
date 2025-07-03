print("\033c\033[43;30m\ngive the file name ?: ")
a=input()
b=a.replace(".ll",".list")
f1=open(a,"r")
c=f1.read()
f1.close()
d=c.split("}")
e=""
for h in d:
    h=h.strip()
    i=h.split("{")
    for j in i:
        j=j.strip()
        e=e+"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
        k=j.split("\n")
        for l in k:
            l=l.strip()
            m=l.split(";")
            m=m[0].strip()
            if m!="":
                n=m.split(",")
                o=n[0].split("=")
                if len(o)>1:
                    n=["<<"]+[o[0]]+[o[1]]+n[1:]
                 
                e=e+"\n"+str(n)

f1=open(b,"w")
f1.write(e)
f1.close()
