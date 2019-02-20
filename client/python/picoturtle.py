import urllib.request
import urllib.parse
import json
import webbrowser


class Turtle:
    """
    proxy to turtle remote api
    """

    def __init__(self, turtle_url="http://localhost:3000"):
        self.turtle_url = turtle_url
        self.name = None
        self.turtle_init()

    def turtle_init(self):
        url = self.turtle_url + '/turtle/create?x=250&y=250'
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        self.name = t['name']
        webbrowser.open(self.turtle_url +
                        '/index.html?details=0&list=0&name=' + self.name)
        return t

    def penup(self):
        url = self.turtle_url + '/turtle/' + self.name + '/penup'
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def pendown(self):
        url = self.turtle_url + '/turtle/' + self.name + '/pendown'
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def stop(self):
        url = self.turtle_url + '/turtle/' + self.name + '/stop'
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def forward(self, d):
        url = self.turtle_url + '/turtle/' + self.name + '/forward?d=' + str(d)
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def left(self, a):
        url = self.turtle_url + '/turtle/' + self.name + '/left?a=' + str(a)
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def right(self, a):
        url = self.turtle_url + '/turtle/' + self.name + '/right?a=' + str(a)
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t

    def pencolour(self, r, g, b):
        url = self.turtle_url + '/turtle/' + self.name + '/pencolour?r=' + str(
            r) + '&g=' + str(g) + '&b=' + str(b)
        res = urllib.request.urlopen(url)
        t = json.loads(res.read().decode('utf-8'))
        return t


if __name__ == "__main__":
    t = Turtle()
    t.penup()
    t.pendown()
    t.pencolour(0, 0, 255)
    for i in range(4):
        t.forward(50)
        t.right(90)
    t.stop()
