import httplib


headers = {"Content-type": "application/json"}
conn = httplib.HTTPConnection("localhost:8001")

# conn.request("POST", "/api/v1/add_node", '{ "node_id": 199 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

# conn.request("POST", "/api/v1/add_node", '{ "node_id": 9999 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

# conn.request("POST", "/api/v1/checkpoint", '{ "node_id": 2 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

# conn.request("POST", "/api/v1/add_node", '{ "node_id": 66666 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

# conn.request("POST", "/api/v1/checkpoint", '{ "node_id": 2 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

conn.request("POST", "/api/v1/restore", '{ "node_id": 2 }', headers)
r = conn.getresponse()
print r.read()
print r.status, r.reason




# conn.request("POST", "/api/v1/checkpoint", '{ "node_id": 2 }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason

# conn.request("POST", "/api/v1/add_edge", '{ "node_a_id": 0, "node_b_id": 2  }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason


# conn.request("POST", "/api/v1/remove_edge", '{ "node_a_id": 0, "node_b_id": 2  }', headers)
# r = conn.getresponse()
# print r.read()
# print r.status, r.reason






