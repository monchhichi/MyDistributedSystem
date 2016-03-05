import httplib


headers = {"Content-type": "application/json"}
conn = httplib.HTTPConnection("localhost:8000")

conn.request("POST", "/api/v1/add_node", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/remove_node", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason


conn.request("POST", "/api/v1/add_node", '{ "node_id": 0 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_node", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 0, "node_b_id": 2  }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason


conn.request("POST", "/api/v1/remove_edge", '{ "node_a_id": 0, "node_b_id": 2  }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason






