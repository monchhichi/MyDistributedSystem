import httplib


headers = {"Content-type": "application/json"}
conn = httplib.HTTPConnection("168.62.177.50:8000")

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

# conn.request("POST", "/api/v1/add_node", '{ "node_id": 10000 }', headers)
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






