# Exploring redis


building/setup with docker guide
``` sh
docker run -d --name redis-stack -p 6379:6379 -p 8001:8001 redis/redis-stack:latest
docker exec -it redis-stack redis-cli
#docker exec -it redis-stack redis-cli

apt-get install python3 python3-pip
pip3 install redis


```

``` python
python3
>>> import redis
>>> r = redis.Redis(host='localhost', port=6379, db=0)
>>> r.set('bo', 'bolle')
True
>>> r.get('bo')
b'bolle'
```
