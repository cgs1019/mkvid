import cgi
import mkvid.log as log
import mkvid.route

class Session(object):
    def __init__(self, config, env, response_cb):
        self.config = config
        self.env = env
        self.response_cb = response_cb

    def run(self):
        method_name = self.env['REQUEST_URI'].split('/')[1]

        method = mkvid.route.route(method_name)
        return method(self.env, self.response_cb)
