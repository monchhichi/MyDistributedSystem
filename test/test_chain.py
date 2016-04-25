import httplib
import time

headers = {"Content-type": "application/json"}
conn0 = httplib.HTTPConnection("localhost:7000")
conn1 = httplib.HTTPConnection("localhost:8000")
conn2 = httplib.HTTPConnection("localhost:9000")

conn0.request("POST", "/api/v1/add_node", '{ "node_id": 1 }', headers)
conn0.getresponse().read()

conn0.request("POST", "/api/v1/add_node", '{ "node_id": 2 }', headers)
conn0.getresponse().read()

conn0.request("POST", "/api/v1/add_node", '{ "node_id": 3 }', headers)
conn0.getresponse().read()

conn0.request("POST", "/api/v1/add_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
conn0.getresponse().read()

conn0.request("POST", "/api/v1/add_edge", '{ "node_a_id": 2, "node_b_id": 3 }', headers)
conn0.getresponse().read()

conn0.request("POST", "/api/v1/add_edge", '{ "node_a_id": 1, "node_b_id": 3 }', headers)
conn0.getresponse().read()


conn1.request("POST", "/api/v1/get_neighbors", '{ "node_id": 1 }', headers)
r = conn1.getresponse()
print r.read()
print r.status, r.reason

conn2.request("POST", "/api/v1/get_neighbors", '{ "node_id": 1 }', headers)
r = conn2.getresponse()
print r.read()
print r.status, r.reason

conn1.request("POST", "/api/v1/get_neighbors", '{ "node_id": 2 }', headers)
r = conn1.getresponse()
print r.read()
print r.status, r.reason

conn2.request("POST", "/api/v1/get_neighbors", '{ "node_id": 2 }', headers)
r = conn2.getresponse()
print r.read()
print r.status, r.reason

conn1.request("POST", "/api/v1/get_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
r = conn1.getresponse()
print r.read()
print r.status, r.reason

conn2.request("POST", "/api/v1/get_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
r = conn2.getresponse()
print r.read()
print r.status, r.reason

conn0.request("POST", "/api/v1/remove_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
conn0.getresponse().read()

conn1.request("POST", "/api/v1/get_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
r = conn1.getresponse()
print r.read()
print r.status, r.reason

conn2.request("POST", "/api/v1/get_edge", '{ "node_a_id": 1, "node_b_id": 2 }', headers)
r = conn2.getresponse()
print r.read()
print r.status, r.reason
