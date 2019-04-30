import urllib.request
import urllib.parse
import json
import webbrowser
import builtins
import sys
import datetime
from pathlib import Path
import os.path
import platform
import os
import stat
import subprocess

PICOTURTLE_WEBCANVAS_RELEASES_URL = 'https://github.com/abhishekmishra/picoturtle-web-canvas/releases/download/'
PICOTURTLE_WEBCANVAS_VERSION_TAG = 'v0.0.10'

# def turtle_request(url):
#     #TODO: Added debug/verbose param to enable request times.
#     #start = datetime.datetime.now().timestamp()
#     res = urllib.request.urlopen(url)
#     t = json.loads(res.read().decode('utf-8'))
#     #end = datetime.datetime.now().timestamp()
#     #delta = end - start
#     #print(url + ' Took ' + str(delta))
#     return t

builtins.t = None


def get_picoturtle_exec_name():
    if platform.system() == 'Linux':
        return 'picoturtle-web-canvas-linux'
    elif platform.system() == 'Windows':
        return 'picoturtle-web-canvas-win.exe'
    else:
        print(platform.system())


def get_default_picoturtle_exec_path():
    return os.path.join(str(Path.home()), get_picoturtle_exec_name())


def download_picoturtle_web_canvas(location=None, force=False):
    toolbar_width = 40
    last_bar = 0

    def download_progress(blocknum, bs, size):
        pct = (blocknum * bs)/size
        bar = int(pct * toolbar_width)
        #print(str(blocknum * bs) + ' of ' + str(size))
        nonlocal last_bar
        for i in range(last_bar, bar):
            sys.stdout.write("-")
            sys.stdout.flush()
        last_bar = bar

    url = PICOTURTLE_WEBCANVAS_RELEASES_URL \
        + PICOTURTLE_WEBCANVAS_VERSION_TAG \
        + '/' + get_picoturtle_exec_name()
    if location == None:
        location = get_default_picoturtle_exec_path()
    if os.path.exists(location) and not force:
        print('File already exists at location -> ' + location)
    else:
        print('Downloading ' + url + ' at -> ' + location)

        # setup toolbar
        sys.stdout.write("[%s]" % (" " * toolbar_width))
        sys.stdout.flush()
        # return to start of line, after '['
        sys.stdout.write("\b" * (toolbar_width+1))

        urllib.request.urlretrieve(url, location, download_progress)
        print('Done.')
    st = os.stat(location)
    os.chmod(location, st.st_mode | stat.S_IEXEC)


def run_picoturtle_web_canvas():
    exec_path = get_default_picoturtle_exec_path()
    subprocess.check_call([exec_path])


class Turtle:
    """
    proxy to turtle remote api
    """

    def __init__(self,
                 name=None,
                 host="127.0.0.1",
                 port="3000",
                 bulk=True,
                 bulk_limit=100,
                 open_browser=True):
        self.turtle_url = "http://" + host + ":" + port
        self.bulk = bulk
        self.bulk_limit = bulk_limit
        self.name = name
        self.open_browser = open_browser
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
                # print(self.commands)
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
        if self.open_browser:
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
        t = self.turtle_request(
            'goto', args=[{'k': 'x', 'v': x}, {'k': 'y', 'v': y}])
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

    def export_img(self, filename):
        t = self.turtle_request(
            'export_img', args=[{'k': 'filename', 'v': filename}])
        return t

    def canvas_size(self, width, height):
        t = self.turtle_request('canvas_size', args=[
            {'k': 'width', 'v': width},
            {'k': 'height', 'v': height}])
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
    return builtins.t.penup()


def pendown():
    return builtins.t.pendown()


def penwidth(w):
    return builtins.t.penwidth(w)


def stop():
    return builtins.t.stop()


def state():
    return builtins.t.state()


def home():
    return builtins.t.home()


def clear():
    return builtins.t.clear()


def forward(d):
    return builtins.t.forward(d)


def back(d):
    return builtins.t.back(d)


def goto(x, y):
    return builtins.t.goto(x, y)


def setx(x):
    return builtins.t.setx(x)


def sety(y):
    return builtins.t.sety(y)


def left(a):
    return builtins.t.left(a)


def right(a):
    return builtins.t.right(a)


def heading(a):
    return builtins.t.heading(a)


def font(f):
    return builtins.t.font(f)


def filltext(text):
    return builtins.t.filltext(text)


def stroketext(text):
    return builtins.t.stroketext(text)


def pencolour(r, g, b):
    builtins.t.pencolour(r, g, b)


def export_img(filename):
    return builtins.t.export_img(filename)


def canvas_size(width, height):
    return builtins.t.canvas_size(width, height)


if __name__ == "__main__":
    pass
    # t = Turtle()
    # t.penup()
    # t.pendown()
    # t.pencolour(0, 0, 255)
    # for i in range(4):
    #     t.forward(50)
    #     t.right(90)
    # t.stop()
    #download_picoturtle_web_canvas()
    #run_picoturtle_web_canvas()
