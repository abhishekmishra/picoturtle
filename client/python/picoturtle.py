import urllib.request
import urllib.parse
import json
import webbrowser
import builtins
import sys
import datetime

# def turtle_request(url):
#     #TODO: Added debug/verbose param to enable request times.
#     #start = datetime.datetime.now().timestamp()
#     res = urllib.request.urlopen(url)
#     t = json.loads(res.read().decode('utf-8'))
#     #end = datetime.datetime.now().timestamp()
#     #delta = end - start
#     #print(url + ' Took ' + str(delta))
#     return t


class Turtle:
    """
    proxy to turtle remote api
    """

    def __init__(self,
                 name=None,
                 host="127.0.0.1",
                 port="3000",
                 bulk=True,
                 bulk_limit=100):
        self.turtle_url = "http://" + host + ":" + port
        self.bulk = bulk
        self.bulk_limit = bulk_limit
        self.name = name
        self.commands = []
        if self.name == None:
            self.turtle_init()

    def turtle_request(self, cmd, args=None, is_obj=False):
        if self.bulk == True and (cmd != 'create'):
            cargs = []
            if args != None:
                if is_obj:
                    arg_obj = {}
                    for i in range(len(args)):
                        arg_obj[args[i]['k']] = args[i]['v']
                    cargs.append(arg_obj)
                else:
                    for i in range(len(args)):
                        cargs.append(args[i]['v'])
            command = {'cmd': cmd, 'args': cargs}
            self.commands.append(command)
            if (len(self.commands) >= self.bulk_limit) or (cmd == 'stop') or (cmd == 'state'):
                #print(self.commands)
                # drain the commands
                req = urllib.request.Request(self.turtle_url + '/turtle/' +
                                             self.name + '/commands')
                req.add_header('Content-Type',
                               'application/json; charset=utf-8')
                jsondata = json.dumps(self.commands)
                jsondataasbytes = jsondata.encode('utf-8')  # needs to be bytes
                req.add_header('Content-Length', len(jsondataasbytes))
                res = urllib.request.urlopen(req, jsondataasbytes)
                # print('Draining ' + str(len(self.commands)) + ' commands for ' +
                #       self.name)
                self.commands = []
                t = json.loads(res.read().decode('utf-8'))
                return t
        else:
            #print('start -> ' + request_url)
            request_url = '/turtle/'
            if self.name != None:
                request_url += self.name
                request_url += '/'
            request_url += cmd
            if args != None:
                request_url += '?'
                for i in range(len(args)):
                    if (i > 0):
                        request_url += '&'
                    request_url += args[i]['k']
                    request_url += '='
                    request_url += str(args[i]['v'])
            res = urllib.request.urlopen(self.turtle_url + request_url)
            t = json.loads(res.read().decode('utf-8'))
            print('done  -> ' + request_url)
            return t

    def turtle_init(self):
        self.commands = []
        t = self.turtle_request(
            'create', args=[{
                'k': 'x',
                'v': 250
            }, {
                'k': 'y',
                'v': 250
            }])
        self.name = t['name']
        webbrowser.open(self.turtle_url +
                        '/index.html?details=0&list=0&name=' + self.name)
        return t

    def penup(self):
        t = self.turtle_request('penup')
        return t

    def pendown(self):
        t = self.turtle_request('pendown')
        return t

    def penwidth(self, w):
        t = self.turtle_request('penwidth', args=[{'k': 'w', 'v': w}])
        return t

    def stop(self):
        t = self.turtle_request('stop')
        return t

    def state(self):
        t = self.turtle_request('state')
        return t

    def home(self):
        t = self.turtle_request('home')
        return t

    def clear(self):
        t = self.turtle_request('clear')
        return t

    def forward(self, d):
        t = self.turtle_request('forward', args=[{'k': 'd', 'v': d}])
        return t

    def back(self, d):
        t = self.turtle_request('back', args=[{'k': 'd', 'v': d}])
        return t

    def goto(self, x, y):
        t = self.turtle_request('goto', args=[{'k': 'x', 'v': x}, {'k': 'y', 'v': y}])
        return t

    def setx(self, x):
        t = self.turtle_request('setx', args=[{'k': 'x', 'v': x}])
        return t

    def sety(self, y):
        t = self.turtle_request('sety', args=[{'k': 'y', 'v': y}])
        return t

    def left(self, a):
        t = self.turtle_request('left', args=[{'k': 'a', 'v': a}])
        return t

    def right(self, a):
        t = self.turtle_request('right', args=[{'k': 'a', 'v': a}])
        return t

    def heading(self, a):
        t = self.turtle_request('heading', args=[{'k': 'a', 'v': a}])
        return t

    def font(self, f):
        t = self.turtle_request('font', args=[{'k': 'f', 'v': f}])
        return t

    def filltext(self, text):
        t = self.turtle_request('filltext', args=[{'k': 'text', 'v': text}])
        return t

    def stroketext(self, text):
        t = self.turtle_request('stroketext', args=[{'k': 'text', 'v': text}])
        return t

    def pencolour(self, r, g, b):
        t = self.turtle_request('pencolour', [{
            'k': 'r',
            'v': r
        }, {
            'k': 'g',
            'v': g
        }, {
            'k': 'b',
            'v': b
        }], True)
        return t


def create_turtle():
    name = None
    port = None
    if len(sys.argv) > 1:
        name = sys.argv[1]
    if len(sys.argv) > 2:
        port = sys.argv[2]

    if port is not None:
        t = Turtle(name, port=port)
        builtins.t = t
    else:
        t = Turtle(name)
        builtins.t = t


def penup():
    return t.penup()


def pendown():
    return t.pendown()


def penwidth(w):
    return t.penwidth(w)


def stop():
    return t.stop()


def state():
    return t.state()


def home():
    return t.home()


def clear():
    return t.clear()


def forward(d):
    return t.forward(d)


def back(d):
    return t.back(d)


def goto(x, y):
    return t.goto(x, y)


def setx(x):
    return t.setx(x)


def sety(y):
    return t.sety(y)


def left(a):
    return t.left(a)


def right(a):
    return t.right(a)


def heading(a):
    return t.heading(a)


def font(f):
    return t.font(f)


def filltext(text):
    return t.filltext(text)


def stroketext(text):
    return t.stroketext(text)


def pencolour(r, g, b):
    t.pencolour(r, g, b)


if __name__ == "__main__":
    t = Turtle()
    t.penup()
    t.pendown()
    t.pencolour(0, 0, 255)
    for i in range(4):
        t.forward(50)
        t.right(90)
    t.stop()
