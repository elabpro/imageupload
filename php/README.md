# Simple image upload API (on PHP)

## How to run

docker-composer run --build

API will be available at http://localhost:8080/api/images

## Documentation

See in docs/.
You can refresh API documentation by using doxygen

Example: cd docs/ && doxygen

Description in russian
![How it works](docs/main.png)

## How to test

cd tests && phpunit testApi.php
