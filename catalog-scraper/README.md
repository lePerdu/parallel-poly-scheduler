# Poly Catalog Scraper

Web scraper for the Florida Polytechnic University course degree program catalog.


## Dependencies

This project uses a
[virtual environment](https://docs.python.org/3/library/venv.html) in order to
minimize dependency problems.

The following packages are used:

- Python 3.5 or greater
- [`lxml`](https://lxml.de/)
- [`requests`](https://3.python-requests.org/)


## Usage

Once inside the virtual environment, run:

``` sh
python scraper.py <catalog_id> <program_id> [<output_file>]
```

The `catalog_id` and `program_id` values can be found by looking at the URL when
viewing a degree program.
