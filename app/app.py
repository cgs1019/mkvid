import mkvid.config
import mkvid.session

def application(env, start_response):
    session = mkvid.session.Session(mkvid.config.prod, env, start_response)
    return session.run()
