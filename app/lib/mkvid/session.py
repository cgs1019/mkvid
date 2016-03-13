import cgi
import xtitch.log as log
import xtitch.model
import xtitch.route

class Session(object):
    def __init__(self, config, env, start_response):
        self.config = config
        self.model = xtitch.model.Model(config)
        self.env = env
        self.start_response = start_response

    def run(self):
        response_headers = []
        auth_info = xtitch.auth.check_auth(self.env, response_headers)

        if auth_info is None:
            response_headers.append(("X-Accel-Redirect", "/splash.html"))
            log.info(response_headers)
            self.start_response("403 Forbidden", response_headers)
            return ""

        response_headers.append(("Content-Type", "text/html"))
        log.info(response_headers)
        self.start_response("200 OK", response_headers)
        return cgi.escape(str(auth_info))

        #method = xtitch.route.route(method_name)
        #return method(self.env, self.start_response)
