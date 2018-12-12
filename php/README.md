# Simple image upload API (on PHP)

## How to run

./prepare.sh && docker-composer run --build

API will be available at http://localhost:8080/api/images

## API

- GET http://localhost:8080/api/images - get JSON array of images
- GET http://localhost:8080/api/images/{id} -get image #id
- POST http://localhost:8080/api/images - upload an image or images
- POST BODY:
  multipart attachments
  url={url for downloading}
- POST BODY for application/json content type
  JSON {
    {name="{filename}",
     content="{base64 content}",
     url="{url}"}, ...
  }

## Documentation

See in docs/.
You can refresh API documentation by using doxygen

Example: cd docs/ && doxygen

Description in russian
![How it works](docs/main.png)

## How to test

cd tests && phpunit testApi.php
