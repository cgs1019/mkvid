import datetime

def print_log(level, msg):
    print("%s %s %s" % (level, datetime.datetime.now(), msg))

def info(msg):
    print_log("INFO", msg)

def error(msg, e=None):
    if e is not None:
        print_log("ERROR", "%s\n%s" % (msg, e))
    else:
        print_log("ERROR", "%s" % msg)
