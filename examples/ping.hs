input = argv[1]

if !net.is_ip(input) {
    ip = net.resolve(input)
} else {
    ip = input
}

if net.ping(ip) {
    print("Host is reachable: ", ip)
} else {
    print("Host ", ip, " is unreachable")
}
