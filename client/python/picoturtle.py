import urllib.request
import urllib.parse
import json
import webbrowser
import builtins
import sys
import datetime


def turtle_request(url):
    #TODO: Added debug/verbose param to enable request times.
    #start = datetime.datetime.now().timestamp()
    res = urllib.request.urlopen(url)
    t = json.loads(res.read().decode('utf-8'))
    #end = datetime.datetime.now().timestamp()
    #delta = end - start
    #print(url + ' Took ' + str(delta))
    return t


class Turtle:
    """
    proxy to turtle remote api
    """

    def __init__(self, name=None, host="127.0.0.1", port="3000"):
        self.turtle_url = "http://" + host + ":" + port
        self.name = name
        if self.name == None:
            self.turtle_init()

    def turtle_init(self):
        t = turtle_request(self.turtle_url + '/turtle/create?x=250&y=250')
        self.name = t['name']
        webbrowser.open(self.turtle_url +
                        '/index.html?details=0&list=0&name=' + self.name)
        return t

    def penup(self):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name + '/penup')
        return t

    def pendown(self):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name +
                           '/pendown')
        return t

    def penwidth(self, w):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name +
                           '/penwidth?w=' + str(w))
        return t

    def stop(self):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name + '/stop')
        return t

    def clear(self):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name + '/clear')
        return t

    def forward(self, d):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name +
                           '/forward?d=' + str(d))
        return t

    def left(self, a):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name +
                           '/left?a=' + str(a))
        return t

    def right(self, a):
        t = turtle_request(self.turtle_url + '/turtle/' + self.name +
                           '/right?a=' + str(a))
        return t

    def pencolour(self, r, g, b):
        url = self.turtle_url + '/turtle/' + self.name + '/pencolour?r=' + str(
            r) + '&g=' + str(g) + '&b=' + str(b)
        t = turtle_request(url)
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
    t.penup()


def pendown():
    t.pendown()


def penwidth(w):
    t.penwidth(w)


def stop():
    t.stop()


def clear():
    t.clear()


def forward(d):
    t.forward(d)


def left(a):
    t.left(a)


def right(a):
    t.right(a)


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
