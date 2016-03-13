import mkvid.log as log
import os

VID_SIZE = os.path.getsize("static/test.mp4")
log.info(VID_SIZE)
with open("static/test.mp4", "rb") as f:
    VID = f.read(VID_SIZE)

FAV_SIZE = os.path.getsize("static/test.mp4")
log.info(FAV_SIZE)
with open("static/favicon.ico", "rb") as f:
    FAV = f.read(FAV_SIZE)

def favicon(env, response_cb):
    response_headers = [
            ("Content-Type", "image/jpeg"),
            ("Content-Length", '%d' % FAV_SIZE)]
    response_cb("200 OK", response_headers)
    return ['']

def vid(env, response_cb):
    if 'HTTP_RANGE' in env:
        range_param = env['HTTP_RANGE']

        range_type, range_spec  = range_param.split('=')
        d

    else:
        begin = 0
        end = VID_SIZE
        length = end - begin 

    log.info(env)
    response_headers = [
            ("Content-Type", "video/mp4"),
            ("Content-Length", '%d' % length)]
    response_cb("200 OK", response_headers)
    return [VID]

def nothing(env, response_cb):
    response_headers = [("Content-Type", "text/html")]
    response_cb("200 OK", response_headers)
    return [' '.join([
        '<video autoplay loop controls>',
        '<source src="vid.mp4" type="video/mp4">',
        '</video>'])]

routes = {
    'favicon.ico': favicon,
    'vid.mp4': vid,
    '': nothing
}

def route(method_name):
    log.info(method_name)
    try:
        return routes[method_name]    
    except KeyError, e:
        log.error("Unrecognized method: %s" % method_name)
