# Memo-client-cli
Command line client for the Memo app. Mainly for testing the [server](https://github.com/Kirtsim/Memo-server) side.

### Before running the client
Make sure the server is running before you start the client. To start the server follow instructions [here](https://github.com/Kirtsim/Memo-server).

### Running the client
Here we assume that docker is installed and the required docker image is present on our machine. See the server [setup guide](https://github.com/Kirtsim/Memo-server) for more info.
```
$ git clone git@github.com:Kirtsim/Memo-client-cli.git
$ cd Memo-client-cli
$ ./run
```
The -run- script builds the project and starts the client. The client can be built without being started and vice versa. Execute `$ ./run help` to see all the options.
If you only need to run the client, run the script as: `$ ./run client`.
