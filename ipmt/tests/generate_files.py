import os

def filterAlpha(a):
    return (a.isalpha() or a == '\n' or a == ' ')

def generate(size) :
    f = open('english.txt', 'r')
    o = open('english'+str(size)+'.txt', 'w')
    lines = f.readlines()
    for l in lines:
        l = filter(filterAlpha, l.replace('\00', '').replace('\0', ''))
        if len(l) > 0:
            o.write(l)
        if os.path.getsize('english'+str(size)+'.txt') > size*1000000:
            break
    f.close()
    o.close()

if __name__ == '__main__':
    generate(5)
    generate(10)
    generate(15)
    generate(20)
    generate(30)
    generate(50)
