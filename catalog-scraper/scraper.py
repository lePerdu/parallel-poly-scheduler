#!/usr/bin/env python

import re
import pprint
import sys
import json

import requests
from lxml import html

ONCLICK_COURSE_ID_RE = re.compile(r"^\w+\('\d+',\s*'(\d+)'")


def scrape_course_id(onclick_handler):
    """Scrapes internal coruse IDs from onclick handlers."""
    return int(ONCLICK_COURSE_ID_RE.match(onclick_handler)[1])


def scrape_major_catalog(catalog_id, program_id):
    catalog_resp = requests.get(
        'http://catalog.floridapoly.edu/preview_program.php',
        params={
            'catoid': catalog_id,
            'poid': program_id,
        })

    catalog_page = html.fromstring(catalog_resp.content)
    course_els = catalog_page.xpath(
        '//div[@class="acalog-core"]/ul/li[@class="acalog-course"]/span')

    courses = []

    for el in course_els:
        title_el = el.xpath('a')[0]
        credits_el = el.xpath('text()[last()]')[0]

        try:
            credits = int(credits_el)
        except:
            continue

        course_name, title = title_el.text.split(' - ')
        course_id = scrape_course_id(title_el.get('onclick'))

        dep, number = course_name.split(' ')

        courses.append({
            'id': course_id,
            'department': dep,
            'number': number,
            'title': title,
            'credits': credits,
        })

    return courses


if __name__ == '__main__':
    courses = scrape_major_catalog(sys.argv[1], sys.argv[2])
    pprint.pprint(courses)

    if len(sys.argv) >= 3:
        with open(sys.argv[3], 'w') as f:
            json.dump(courses, f)

