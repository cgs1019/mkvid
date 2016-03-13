import psycopg2 as pg
import xtitch.auth
import xtitch.db

def user_info(params, env, response_callback):
    auth_info = xtitch.auth.check_auth(env)
    conn = pg.connect("dbname = xtitch")
    cur = conn.cursor()

    column_names = ("XtitcherName", "FullName")
    cur.execute(
        """SELECT %s, %s
           FROM Xtitcher
           WHERE XtitcherName = %s;""",
        column_names + (auth_info['XtitcherName']))

    result = cur.fetchone()
    if result is not None:
        return dict(zip(column_names, result))
    else:
        raise ValueError("No data found for XtitcherName %s" % (
            auth_info["XtitcherName"]))
