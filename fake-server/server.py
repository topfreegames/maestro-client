from flask import Flask
from flask import request

app = Flask(__name__)

@app.route("/scheduler/<scheduler_name>/rooms/<room_name>/address")
def address_polling(scheduler_name, room_name):
    return '{"success":true, "addresses": [{"address":"localhost:6379","name":"aeah"},{"address":"localhost:6378","name":"aeah2"}]}'

@app.route("/scheduler/<scheduler_name>/rooms/<room_name>/ping", methods=['PUT'])
def ping(scheduler_name, room_name):
    print "ping called with data: {0}".format(request.data)
    return '{"success":true}'

@app.route("/scheduler/<scheduler_name>/rooms/<room_name>/status", methods=['PUT'])
def update_status(scheduler_name, room_name):
    print "update status called with data: {0}".format(request.data)
    return '{"success":true}'

if __name__ == "__main__":
    app.run(host='0.0.0.0')
