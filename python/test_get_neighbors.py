import httplib


headers = {"Content-type": "application/json"}
conn = httplib.HTTPConnection("localhost:8000")

conn.request("POST", "/api/v1/add_node", '{ "node_id": 1 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_node", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_node", '{ "node_id": 3 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_node", '{ "node_id": 4 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_node", '{ "node_id": 5 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 1, "node_b_id": 3 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 1, "node_b_id": 4 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 2, "node_b_id": 3 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/get_neighbors", '{ "node_id": 1 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/get_neighbors", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/get_neighbors", '{ "node_id": 3 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/get_neighbors", '{ "node_id": 4 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

conn.request("POST", "/api/v1/get_neighbors", '{ "node_id": 5 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason

