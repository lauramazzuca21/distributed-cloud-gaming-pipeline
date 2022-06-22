# Headless Renderer
## Dependencies
>For the libraries needed to run locally this application see the [Dockerfile](https://github.com/lauramazzuca21/distributed-cloud-gaming-pipeline/blob/master/headless-renderer/Dockerfile).

The host to run the Docker container onto needs to have a NVIDIA GPU and install the NVIDIA Docker toolkit with the following commands:
```bash
$ distribution=$(. /etc/os-release;echo $ID$VERSION_ID) \
   && curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add - \
   && curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list
   
$ sudo apt-get update && sudo apt-get install -y nvidia-container-toolkit

$ sudo service docker stop
$ sudo tee /etc/docker/daemon.json <<EOF
{
    "runtimes": {
        "nvidia": {
            "path": "/usr/bin/nvidia-container-runtime",
            "runtimeArgs": []
        }
    },
		"default-runtime": "nvidia"
}
EOF
$ sudo service docker start
```
## Application
### Local setup
To install the application locally run:
```bash
cd headless-renderer
cmake .
make
```
then, to run it:
```bash
./main
```

### Docker container setup
To install from a clone of this repository
```bash
sudo docker build . -t headless-renderer:latest
```
To download from [docker hub](https://hub.docker.com/repository/docker/lauramazzuca/headless-renderer)
```bash
docker pull lauramazzuca/headless-renderer:latest
```

Then run it with host network configuration with
```bash
sudo docker run --rm --network host -dit headless-renderer:latest
```
