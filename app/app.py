import mkvid.config
import mkvid.session

def application(env, response_cb):
    session = mkvid.session.Session(mkvid.config.prod, env, response_cb)
    return session.run()
