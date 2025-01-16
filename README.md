# Network Scanner

This project is a multithreaded network scanner built in C++. It scans a specified range of ports on a target IP address and provides detailed information about open ports, including the service name and response time.

## Features

- **Multithreading**: Efficiently scans multiple ports concurrently with user-defined thread limits.
- **Service Detection**: Identifies the service running on open ports using the system's service database.
- **Response Time Measurement**: Reports the time taken to connect to each port.
- **Thread-Safe Output**: Ensures readable and synchronized console output.

## Requirements

- A Linux-based operating system (or WSL on Windows)
- g++ compiler

## Compilation

Use the following command to compile the program:
```bash
g++ -std=c++11 -pthread -o network_scanner network_scanner.cpp
```

## Usage

Run the compiled executable and follow the prompts:
```bash
./network_scanner
```

### Input

1. **Target IP Address**: The IP address of the host you want to scan.
2. **Start Port**: The starting port number of the range to scan.
3. **End Port**: The ending port number of the range to scan.
4. **Maximum Threads**: The maximum number of threads to use during the scan.

### Output

For each open port, the program will display:
- Port number
- Service name (if available)
- Response time in seconds

### Example

```plaintext
Enter the target IP address: 192.168.1.1
Enter the start port: 20
Enter the end port: 100
Enter the maximum number of threads: 10
Port 22 is open on 192.168.1.1 (Service: ssh, Response Time: 0.0012s)
Port 80 is open on 192.168.1.1 (Service: http, Response Time: 0.0009s)
Scanning complete!
```

## Disclaimer

This tool is intended for educational purposes only. Scanning networks without proper authorization is illegal and unethical. Always ensure you have permission before using this tool.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests to improve this tool.

## Author

Developed by [Your Name].

