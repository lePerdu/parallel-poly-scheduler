#!/usr/bin/env python

import re
import pprint
import sys
import json

import requests
from lxml import html

ONCLICK_COURSE_ID_RE = re.compile(r"^\w+\('\d+',\s*'(\d+)'")
PREVIEW_COURSE_ID_RE = re.compile(r'coid=(\d+)')


def scrape_course_id(onclick_handler):
    """Scrapes internal coruse IDs from onclick handlers."""
    return int(ONCLICK_COURSE_ID_RE.match(onclick_handler)[1])


def scrape_course_id_preview(course_el):
    return int(PREVIEW_COURSE_ID_RE.search(course_el.get('onclick'))[1])


def scrape_course_prereqs(session, catalog_id, course_id):
    course_resp = requests.get(
        'http://catalog.floridapoly.edu/ajax/preview_course.php',
        params={
            'catoid': catalog_id,
            'coid': course_id,
            'show': '',
        })

    course_page = html.fromstring(course_resp.content)

    # <a> nodes between 2nd and 3rd <strong> elements
    prereq_els = course_page.xpath('''
        //td[@class="coursepadding"]/div/a[
            count(preceding-sibling::strong) = 2
        ]
    ''')

    return (scrape_course_id_preview(el) for el in prereq_els)


def scrape_major_catalog(session, catalog_id, program_id):
    catalog_resp = session.get(
        'http://catalog.floridapoly.edu/preview_program.php',
        params={
            'catoid': catalog_id,
            'poid': program_id,
        })

    catalog_page = html.fromstring(catalog_resp.content)
    course_els = catalog_page.xpath(
        '//div[@class="acalog-core"]/ul/li[@class="acalog-course"]/span')

    courses = {}

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

        courses[course_id] = {
            'department': dep,
            'number': number,
            'title': title.strip(),
            'credits': credits,
        }

    return courses


def scrape_catalog_and_prereqs(catalog_id, program_id):
    with requests.Session() as session:
        courses = scrape_major_catalog(session, catalog_id, program_id)

        for course_id, course in courses.items():
            prereqs = scrape_course_prereqs(session, catalog_id, course_id)

            # Filter out prerequisites not in this degree program
            course['prereqs'] = [p for p in prereqs if p in courses]

        return courses


if __name__ == '__main__':
    courses = scrape_catalog_and_prereqs(sys.argv[1], sys.argv[2])
    pprint.pprint(courses)

    if len(sys.argv) >= 3:
        with open(sys.argv[3], 'w') as f:
            json.dump(courses, f)
