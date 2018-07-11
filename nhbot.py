import requests, re, random
from random import getrandbits
from time import sleep
import urllib2

from selenium import webdriver
from selenium.webdriver.common.keys import Keys


url = 'url'
email_head = 'email'

word_site = 'dictionary url'

response = urllib2.urlopen(word_site)
txt = response.read()
WORDS = txt.splitlines()

def main(limit):
    f = open('emails.txt','a') #saves emails in textfile


    for n in range(limit):
        word=random.choice(WORDS)
        email = '{}{}@domain'.format(word, getrandbits(10))
        # payload ={
        #     'u': '98aba7d0141269a44537b7a58',
        #     'amp;id': '5e3e8d54ab',
        #     'EMAIL' : email
        # }
        # resp = requests.post(url, data = payload)
        driver = webdriver.Chrome(executable_path='/path/to/driver')
        driver.get(url)
        elem = driver.find_element_by_name(name)
        elem.send_keys(email)
        f.write(email+'\n')

        driver.find_element_by_id(submit).click()
        sleep(5)
        driver.quit()



        # if any(re.findall(r'success', str(resp.text), re.IGNORECASE)):
        #     print "success with {}".format(email)

        # else:
        #     print(email)
        #     print "failed"
        #     print resp.content
        #     exit()
    f.close()
main(30)
exit()
