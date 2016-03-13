import mkvid.log as log

def asdf(env):
    content_length = int(env["CONTENT_LENGTH"])
    content = env["wsgi.input"].read(content_length)

    params = urlparse.parse_qs(content)
    log.info(str(env))
    log.info(str(params))

routes = {
    'asdf': asdf
}

def route(method_name):
    log.info("Heyyyy")
    try:
        return routes[method_name]    
    except KeyError, e:
        log.error("Unrecognized method: %s" % method_name)
