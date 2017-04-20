from flask import Flask
from flask import request

app = Flask(__name__)

@app.route("/namespaces/<namespace_name>/rooms/<room_name>/address")
def address_polling(namespace_name, room_name):
    return '{"host":"localhost","port":6379,"success":true}'

@app.route("/namespaces/<namespace_name>/rooms/<room_name>/ping", methods=['PUT'])
def ping(namespace_name, room_name):
    return '{"success":true}'

@app.route("/namespaces/<namespace_name>/rooms/<room_name>/status", methods=['PUT'])
def update_status(namespace_name, room_name):
    print "update status called with data: {0}".format(request.data)
    return '{"success":true}'

if __name__ == "__main__":
    app.run()
