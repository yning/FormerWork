///////////////////////////////////////////////////////////////
// SearchEngine.cpp: implementation of the CSearchEngine class.
//
//
///////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <iostream>
#include <fstream>
using namespace std;


#include "SearchEngine.h"
#include "Url.h"

#ifdef   _DEBUG 
#define  new   DEBUG_NEW 
#undef   THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif
struct engine_entry search_engines[NUM_SEARCH_ENGINES] = 
{
	{"7metasearch.com", "q"},			{"7search.com", "qu"},			{"aolsearch.aol.com", "query"},
	{"asia.google.yahoo.com", "p"},		{"asia.ink.yahoo.com", "p"},	{"asia.search.yahoo.com", "p"},
	{"au.chopstix.net", "search"},		{"au.google.yahoo.com", "p"},	{"au.ink.yahoo.com", "p"},
	{"au.search.yahoo.com", "p"},		{"auto.search.msn.com", "q"},	{"autobrand.wunderground.com", "q,search"},
	{"av.yahoo.com", "p"},				{"br.busca.yahoo.com", "p"},	{"britannica.directhit.com", "qry"},
	{"buscador.elsitio.com", "palabras"},	{"buscaweb.starmedia.com", "query"},		{"ca.google.yahoo.com", "p"},
	{"ca.search.yahoo.com", "p"},			{"chinese.excite.com", "search"},			{"cissearch.compuserve.com", "sTerm"},
	{"click.hotbot.com", "query"},			{"click.linksynergy.com", "q"},				{"cnn.looksmart.com", "key"},
	{"crawler.nana.co.il", "string"},		{"db.education-world.com", "queryText"},	{"de.altavista.com", "q"},
	{"de.search.yahoo.com", "p"},			{"de.search.yahoo.com", "p"},				{"dir.hotbot.lycos.com", "MT"},
	{"directory.netscape.com", "search"},	{"dk.search.yahoo.com", "p"},				{"dogpile.com", "q"},
	{"earch.msn.com", "MT,q"},				{"english.nana.co.il", "string"},			{"evreka.passagen.se", "q"},
	{"excite.au.netscape.com", "search"},	{"excite.co.uk", "search"},					{"excite.de.netscape.com", "search"},
	{"excite.fr.netscape.com", "search"},	{"excite.uk.netscape.com", "search"},		{"findia.net", "query"},
	{"fr.excite.com", "search"},			{"fr.search.yahoo.com", "p"},				{"google.netscape.com", "query,q"},
	{"google.yahoo.com", "p"},				{"goto.earthlink.net", "Keywords"},			{"gotonet.google.com", "q"},
	{"hke.google.yahoo.com", "p"},			{"hotbot.lycos.com", "MT"},					{"in.google.yahoo.com", "p"},
	{"in.search.yahoo.com", "p"},			{"infoseek.go.com", "qt"},					{"ink.nomade.fr", "MT"},
	{"ink.yahoo.com", "p"},					{"it.altavista.com", "q"},					{"ixquick.com", "query"},
	{"ixquick.com", "query"},				{"janas.tiscalinet.it", "query"},			{"job.com", "Keywords"},
	{"kevxml.infospace.com", "qkw"},		{"listings.altavista.com", "q"},			{"lycospro.lycos.com", "query"},
	{"mamma.com", "query"},					{"meta.naver.com", "query,oldquery"},		{"miner.bol.com.br:8000", "q"},
	{"msn.achla.co.il", "KeyWords"},		{"msn.directhit.com", "qry"},				{"msn.nana.co.il", "string"},
	{"multi1.ceoexpress.com", "DTqb1"},		{"mundial.sapo.pt", "kw"},					{"mx.search.yahoo.com", "p"},
	{"nbci.com", "keyword"},				{"netfind.aol.com", "query"},				{"news.icqit.com", "query"},
	{"nl.altavista.com", "q"},				{"nl.excite.com", "search"},				{"no.search.yahoo.com", "p"},
	{"nodsearch.netscape.com", "search"},	{"partners.mamma.com", "query"},			{"pre.sympatico.ca", "query"},
	{"ragingsearch.altavista.com", "q"},	{"rd.lycos.de", "qry"},						{"ricerca.inwind.it", "B1"},
	{"s5.debriefing.ixquick.com", "query"},	{"s9.ixquick.com", "query"},				{"se.excite.com", "search"},
	{"se.search.yahoo.com", "p"},			{"search.about.com", "terms"},				{"search.aol.ca", "query"},
	{"search.aol.com", "query"},			{"search.aroob.com", "q"},					{"search.asia.yahoo.com", "p"},
	{"search.atomz.com", "sp-q"},			{"search.bluewin.ch", "q"},					{"search.btinternet.com", "MT"},
	{"search.chinese.yahoo.com", "p"},		{"search.cnet.com", "QUERY"},				{"search.com", "q"},
	{"search.curryguide.com", "query"},		{"search.dmoz.org", "search"},				{"search.empas.com", "q"},
	{"search.espanol.yahoo.com", "p"},		{"search.excite.com", "search,s"},			{"search.fr.msn.ca", "MT"},
	{"search.ft.com", "query"},				{"search.go2net.com", "general"},			{"search.icq.com", "query"},
	{"search.indiainfo.com", "MT"},			{"search.iwon.com", "searchfor"},			{"search.latam.msn.com", "MT"},
	{"search.mciworld.com", "sTerm"},		{"search.megaspider.com", "q"},				{"search.metacrawler.com", "general"},
	{"search.msn.be", "MT"},				{"search.msn.ca", "MT,q"},					{"search.msn.co.nz", "MT"},
	{"search.msn.co.uk", "MT,q"},			{"search.msn.com", "MT,q"},					{"search.msn.com.mx", "MT"},
	{"search.msn.com.sg", "MT,q"},			{"search.msn.de", "MT"},					{"search.msn.it", "MT"},
	{"search.msn.se", "MT"},				{"search.netscape.com", "search"},			{"search.netscapeonline.co.uk", "query"},
	{"search.ninemsn.com.au", "q"},			{"search.oingo.com", "s"},					{"search.rediff.com", "MT"},
	{"search.searchalot.com", "query"},		{"search.searchfuel.com", "sc"},			{"search.sify.com", "q"},
	{"search.sli.sympatico.ca", "query"},	{"search.sol.dk", "query"},					{"search.sol.no", "q,query"},
	{"search.sprinks.about.com", "terms"},	{"search.ukmax.com", "MT"},					{"search.virgilio.it", "qs"},
	{"search.walla.co.il", "q"},			{"search.weather.yahoo.com", "p"},			{"search.yahoo.com", "p"},
	{"searchopolis.com", "request"},		{"sg.ink.yahoo.com", "p"},					{"sg.search.yahoo.com", "p"},
	{"snap.com", "keyword"},				{"suche.fireball.de", "q"},					{"suchen.abacho.de", "q"},
	{"systema.directhit.com", "qry"},		{"szukaj.wp.pl", "szukaj"},					{"uk.altavista.com", "q"},
	{"uk.google.yahoo.com", "p"},			{"uk.ink.yahoo.com", "p"},					{"uk.search.yahoo.com", "p"},
	{"virtualfish.com", "query"},			{"websearch.cs.com", "sTerm"},				{"websearch.infinit.com", "qt"},
	{"woa.metagopher.com", "w"},			{"world.voila.com", "kw"},					{"www.1blink.com", "q"},
	{"www.4arabs.com", "query"},			{"www.accufind.com", "keywords"},			{"www.achla.co.il", "keyword"},
	{"www.admaiora.com", "search"},			{"www.allthesites.com", "query"},			{"www.alltheweb.com", "query"},
	{"www.altavista.com", "q"},				{"www.altavista.de", "q"},					{"www.ameinfo.com", "qry"},
	{"www.americaonline.com.br", "query"},	{"www.anzwers.com.au", "query"},			{"www.ask.co.uk", "ask"},
	{"www.ask.com", "ask"},					{"www.askjeeves.com", "ask"},				{"www.askjeeveskids.com", "ask"},
	{"www.brightgate.com", "q"},			{"www.brint.com", "PA"},					{"www.business.com", "query"},
	{"www.c4.com", "SearchText"},			{"www.canada.com", "QRY"},					{"www.chemedia.com", "query"},
	{"www.chemweb.com", "keywords"},		{"www.debriefing.com", "keyword"},			{"www.directhit.com", "qry"},
	{"www.dtcc.edu", "q"},					{"www.elcomsoft.com", "s"},					{"www.ensino.com.br", "palavra"},
	{"www.eresmas.com", "q"},				{"www.euroseek.com", "query"},				{"www.evreka.com", "q"},
	{"www.evreka.fi", "q"},					{"www.excite.at", "search"},				{"www.excite.ca", "s,search"},
	{"www.excite.ch", "search"},			{"www.excite.co.jp", "s"},					{"www.excite.co.uk", "search"},
	{"www.excite.com.au", "search"},		{"www.excite.de", "search"},				{"www.excite.dk", "search"},
	{"www.excite.es", "s"},					{"www.excite.fr", "search"},				{"www.excite.it", "search"},
	{"www.france.com", "search"},			{"www.gnet.com", "search"},					{"www.go.com", "qt"},
	{"www.goeureka.com.au", "q"},			{"www.gogo.co.il", "T"},					{"www.gohip.com", "sc"},
	{"www.goo.ne.jp", "MT"},				{"www.google.ca", "q,query"},				{"www.google.com.hk", "q,query"},//CHANGE BY NY;{"www.goo.ne.jp", "MT"},{"www.google.ca", "q,query"},{"www.google.com", "q,query"},
	{"www.goto.com", "Keywords"},			{"www.guidebeam.com", "query"},				{"www.highway61.com", "string"},
	{"www.homepageware.com", "search"},		{"www.hotbot.com", "MT"},					{"www.hotbot.lycos.com", "MT"},
	{"www.icqit.com", "query"},				{"www.ifind.freeserve.com", "q"},			{"www.ifind.freeserve.net", "q"},
	{"www.iguide.co.il", "search"},			{"www.ilse.nl", "search_for"},				{"www.indiatimes.com", "q,query"},
	{"www.infind.com", "query"},			{"www.infoseek.com", "qt"},					{"www.infoseek.de", "qt"},
	{"www.intelliseek.com", "queryterm"},	{"www.interfree.it", "query"},				{"www.israel.com", "search"},
	{"www.ixquick.com", "query"},			{"www.jewish.com", "query"},				{"www.jumpforce.com", "Keywords"},
	{"www.jumpy.it", "searchWord"},			{"www.kanoodle.com", "query"},				{"www.lineone.net", "query"},
	{"www.looksmart.co.uk", "key"},			{"www.looksmart.com", "key"},				{"www.looksmart.com.au", "key"},
	{"www.lycos.com", "query"},				{"www.lycos.de", "query"},					{"www.lycos.fr", "query"},
	{"www.lycos.it", "query"},				{"www.lycos.nl", "query"},					{"www.lycos.se", "query"},
	{"www.meta360.com", "qry"},				{"www.metacrawler.com", "general"},			{"www.metacrawler.de", "qry"},
	{"www.metaeureka.com", "terms"},		{"www.monstercrawler.com", "qry"},			{"www.musicsearch.com", "QUERY"},
	{"www.nana.co.il", "string"},			{"www.net2day.com", "search"},				{"www.netbul.com", "keyword"},
	{"www.netex.co.il", "SEARCHSTRING"},	{"www.northernlight.com", "qr"},			{"www.one2seek.com", "qry"},
	{"www.profusion.com", "queryterm"},		{"www.radaruol.com.br", "MT"},				{"www.rdn.ac.uk", "query"},
	{"www.redbox.cz", "srch"},				{"www.savvy.com", "s"},						{"www.savvysearch.com", "q"},
	{"www.searchalot.com", "q"},			{"www.searchbug.com", "query"},				{"www.searchcactus.com", "SearchString"},
	{"www.searcheurope.com", "query"},		{"www.secondpower.com", "Keywords"},		{"www.shinyseek.it", "KEY"},
	{"www.snoopa.com", "search"},			{"www.sosig.ac.uk", "terml"},				{"www.spinneweb.nl", "keyword"},
	{"www.start.co.il", "searchString"},	{"www.startingpage.com", "query"},			{"www.startpage.com", "query"},
	{"www.suchmaschine.com", "suchstr"},	{"www.sureseeker.com", "search"},			{"www.surfwax.com", "words"},
	{"www.thebighub.com", "QueryText"},		{"www.theyellowpages.com", "search"},		{"www.turkmeta.com", "query"},
	{"www.vindex.nl", "in"},				{"www.voila.com", "kw"},					{"www.webcrawler.com", "search,searchtext"},
	{"www.webhelp.com", "Keywords"},		{"www.webtop.com", "QUERY"},				{"www.work.com", "searchtext"},
	{"www.yarrawongasc.vic.edu.au", "search_term"},		{"www.yeah.net", "key"},		{"www.zindiana.com", "q"},		
	{"www.zooloo.co.il", "search"},			{"zoek.vinden.nl", "query"},				{"zoek.wish.net", "q"}
};


int cmpsearch(void **pfirst, const void *psecond)
{
	const engine_entry *pcmp = (engine_entry *) psecond; 
	const char * pstr1 = (const char *) *pfirst; 
	const char * pstr2 = pcmp->m_engine; //extract string from struct

	return strcmp(pstr1, pstr2);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSearchEngine::CSearchEngine()
{
	m_bInited = false;

	m_bRelatedQuery = false;
	m_bLinkQuery = false;
	m_bCacheQuery = false;
}


CSearchEngine::~CSearchEngine()
{
}


struct engine_entry * CSearchEngine::lookup(const char *host)
{
   struct engine_entry *result = NULL;

   // Find the word using a binary search algorithm
   result = (struct engine_entry *)bsearch( 
	   (char *) &host, 
	   (struct engine_entry *)search_engines, 
	   NUM_SEARCH_ENGINES,
       sizeof( struct engine_entry ), 
	   (int (*)(const void*, const void*))cmpsearch );

   return result;
}

bool CSearchEngine::isSearchEngine(const char *host)
{
	return (lookup(host) != NULL);
}

void CSearchEngine::parseURL(const char *m_url, CTextProcessor *m_textProcessor)
{
	struct engine_entry *pEntry;
	CString m_keyWords;

	m_bSearch = false;
	m_queryKeyWords.RemoveAll();

	m_bRelatedQuery = false;
	m_bLinkQuery = false;

	CUrl m_pURL(m_url);
	if((pEntry = lookup(m_pURL.getHostName())) != NULL)
	{
		m_bSearch = true;

		parseSearchQuery(m_url, (char *)pEntry->m_queryterm, m_keyWords);
		m_textProcessor->textProcess(m_keyWords, m_queryKeyWords);
	}
}



/* Parse the referrer, extract its search engine and return words */
void CSearchEngine::parseSearchQuery(const char *refurl, const char *m_qterm, 
									 CString& m_keywords)
{
	char *buffer, *tengine, *querytext, *qmark, *s, *t, *fullterm;
	char *deliminator, *pkeyword;

	if(refurl == (char *)NULL)
		return;

	buffer = (char *)new char[strlen(refurl) + 1];
	strcpy(buffer, refurl);
	tengine = buffer;
	
	qmark = strchr(buffer, '?');
	if( qmark == NULL)
	{
		delete buffer;
		return;
	}
	
	// match the search engine with the string before '?'
	// Find the search key word after '?', by using term
	*qmark = '\0';
	querytext = qmark + 1;
	
	fullterm = NULL;

	// we have found the search engine
	s = (char *) new char[strlen(m_qterm) + 1];
	strcpy(s, m_qterm);  /* can't strtok arg1 directly in case it's const */
	if ((t = strtok(s, ",")) != NULL)
	{
		// test each query term
		for ( ; t != NULL; t = strtok((char *)NULL, ","))
		{
			fullterm = (char *)new char[strlen(t) + 1 + 1];
			strcpy(fullterm, t);
			fullterm[strlen(t)] = '=';
			fullterm[strlen(t) + 1] = '\0';
				
			pkeyword = strstr(querytext, fullterm); 
			if(pkeyword == NULL)
			{
				delete fullterm;
				fullterm = NULL;
				continue;
			}
				
			pkeyword += strlen(fullterm);
			if((deliminator = strchr(pkeyword, '&')) != NULL)
				*deliminator = '\0';

			m_keywords = pkeyword;

			delete s;
			delete buffer;
			delete fullterm;

			if(m_keywords.Find("related:", 0) > -1)
				m_bRelatedQuery = true;

			if(m_keywords.Find("link:", 0) > -1)
				m_bLinkQuery = true;

			if(m_keywords.Find("cache:", 0) > -1)
				m_bLinkQuery = true;

			if(m_bRelatedQuery || m_bLinkQuery || m_bCacheQuery)
				m_keywords.Empty();

			//decode_string(result);
			int i, j;
//			bool bAfterSpace, bAfterMinus;

			for(i = 0; i < m_keywords.GetLength(); i++)
			{
				if((m_keywords[i] == '+') || (m_keywords[i] == '\n'))
					m_keywords.SetAt(i, ' ');
			}

			// we remove the options of the query, such as
			// -filetype:pdf, ...
			i = 0;
			while ( i < m_keywords.GetLength())
			{
				if(m_keywords[i] == ' ')
				{
					++i;

					if(i < m_keywords.GetLength() && m_keywords[i] == '-')
					{
						for(j = i; j < m_keywords.GetLength(); j++)
						{
							if(m_keywords[j] == ' ')
								break;
							else
								m_keywords.SetAt(j, ' ');
						}

						i = j;
					}
				}
				else
					++i;
			}

			m_keywords.TrimRight();

			return;
		}
	}

	delete s;
	delete buffer;

	if(fullterm)
		delete fullterm;

	return;
}




const char *m_searchEngines[] = {
    "www.google.com.hk",//change by ny;"www.google.com"
    "www.google.ca",
    "www.google.ch",
    "www.google.com.au",
    "www.google.co.jp",
    "www.google.co.uk",
    "www.google.co.th",
    "www.google.ie",
    "www.google.ro",
    "www.google.de",
    "search.excite.com",
    "search.weather.yahoo.com",
    "www.hotbot.lycos.com",
    "search.yahoo.com",
    "google.yahoo.com",
    "auto.search.msn.com",
    "search.atomz.com",
    "www.altavista.com",
    "web.ask.com",
    NULL
};



void CSearchEngine::parseQueryResult(const char *m_url, CPageTagNode *m_pRoot, CTextProcessor *m_processor,
									 List *m_results)
{
    int m_search;
	CUrl pUrl(m_url);

	m_queryResults = m_results;
	m_textProcessor = m_processor;

	// locate the search engine
    m_search = 0;
    while(m_searchEngines[m_search])
    {
        if(stricmp(m_searchEngines[m_search], pUrl.getHostName()) == 0)
            break;
        ++m_search;
    }

	// process the search result according to the search engine
    switch (m_search) {

        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
//  		www.google.com : 2752
//	    	www.google.ca : 22
    		ProcessGoogleCom(m_pRoot);
            break;
        case 10:
//    		search.excite.com : 13
    		ProcessExciteCom(m_pRoot);
            break;
        case 11:
//    		search.weather.yahoo.com : 6
    		ProcessWeatherYahoo(m_pRoot);
            break;
        case 12:
//	    	www.hotbot.lycos.com : 9
    		ProcessHotbotLycos(m_pRoot);
            break;
        case 13:
//    		search.yahoo.com : 145
	    	ProcessSearchYahoo(m_pRoot);
            break;
        case 14:
//	    	google.yahoo.com : 26
    		ProcessGoogleYahoo(m_pRoot);
            break;
        case 15:
//	    	auto.search.msn.com : 19
    		ProcessSearchMsn(m_pRoot);
            break;
        case 16:
//	    	search.atomz.com : 7
    		ProcessAtomzCom(m_pRoot);
            break;
        case 17:
//    		www.altavista.com : 39
	    	ProcessAltaVista(m_pRoot);
            break;
        case 18:
//    		www.askjeeves.com : 1
	    	ProcessAskJeeves(m_pRoot);
            break;

        default:
            break;
    }
}

void CSearchEngine::insertQueryResult(CString url, CString title, CString snippet, bool bSpan, int pageSize)
{
	CQueryResult *pQuery;
	CStringArray m_titles;
	CStringArray m_snippets;
	CString strBuffer;

	m_titles.RemoveAll();
	m_snippets.RemoveAll();

	if(!title.IsEmpty())
		m_textProcessor->textProcess(title, m_titles);

	if(!snippet.IsEmpty())
		m_textProcessor->textProcess(snippet, m_snippets);

	pQuery = (CQueryResult *) new CQueryResult(url, m_titles, m_snippets, bSpan, pageSize);
	pQuery->setTitle(title);
	pQuery->setSnippet(snippet);
	m_queryResults->add(pQuery);

}


/*

There are 12 main file types searched by Google in addition to standard web 
formatted documents in HTML. The most common formats are PDF, PostScript, 
Microsoft Office formats:

Adobe Portable Document Format (pdf) 
Adobe PostScript (ps) 
Lotus 1-2-3 (wk1, wk2, wk3, wk4, wk5, wki, wks, wku) 
Lotus WordPro (lwp) 
MacWrite (mw) 
Microsoft Excel (xls) 
Microsoft PowerPoint (ppt) 
Microsoft Word (doc) 
Microsoft Works (wks, wps, wdb) 
Microsoft Write (wri) 
Rich Text Format (rtf) 
Text (ans, txt) 
Google is also scouring the Web for additional file types that are very rare. 
You may see them pop up in your results from time to time. 

If you prefer to see a particular set of results without a particular file type 
included (for example, PDF), simply type 
-filetype:pdf
within the search box along with your search term(s). 

*/

void CSearchEngine::ProcessGoogleCom(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *content;
	CPageTagNode *pBlockQuote = NULL;
    CString wname;
	/*
	CPageTagNode *body, *content, *div, *atag, *spantag;
	CPageTagNode *pcontent;
	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;
	bool bSpan;
	CPageTagNode *pBlockQuote = NULL;
	*/

	if((body = document->FindBody()) == NULL)
		return;
    wname.Empty();

   //const char filename[] = "D:\\WebMindCode\\Debug\\mytext.txt";
    //ofstream o_file;
   // ifstream i_file;
   // string out_text;
   
    //д
   // o_file.open(filename);
   // o_file << (LPCTSTR) body->m_innerText;

  //  o_file.close();
	
	content = body->findSearch(body); 

     if((content!=NULL)&&(stricmp("",content->m_innerText)!=NULL))
	 {
        GoogleSearchResult(content);
	 }
	
/*
if(m_bRelatedQuery)
	{
		pBlockQuote = body->getFirst("div", 1);//pBlockQuote = body->getFirst("blockquote", 1);

		if(pBlockQuote&&pBlockQuote->checkForResultsDiv(pBlockQuote))
			content = pBlockQuote->getFirst("div", 1);
		else ;
	}
	else
	{
		content = body->getFirst("div", 1);
	}
	

	if(content == NULL)
		return;

	GoogleSearchResult(content);
	 
*/
	/*
	for (content = body->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "blockquote") == 0)
				pBlockQuote = content;

			if(stricmp(wname, "div") == 0)
				break;
		}
	}

	if(content == NULL)
	{
		if(pBlockQuote)
		{
			content = pBlockQuote->getFirst("div", 1);
			if(content == NULL)
				return;
		}
		else
			return;
	}

	div = content;
	for (content = div->getContent(); content != NULL; content = content->getNext())
	{
		content->getName(wname);
		if(stricmp(wname, "blockquote") != 0 && stricmp(wname, "p") != 0)
			continue;

		if(stricmp(wname, "blockquote") == 0)
			pcontent = content->getContent();
		else
			pcontent = content;

		// div contains a, br and font sections
		atag = pcontent->getContent();
		atag->getName(wname);
        while(atag != NULL && stricmp(wname, "a") != 0)
        {
            atag = atag->getNext();
            if(atag)
       		    atag->getName(wname);
        }

        if(atag == NULL)
            continue;

		// to check whether it is an external file format
		spantag = pcontent->getContent();
		spantag->getName(wname);
        while(spantag != NULL && stricmp(wname, "span") != 0)
        {
            spantag = spantag->getNext();
            if(spantag)
       		    spantag->getName(wname);
        }
		bSpan = false;
		if(spantag)
			bSpan = true;

		title.Empty();
		atag->getText(title);

		wtext.Empty();
		pcontent->getText(wtext);

		href.Empty();
		atag->getAttrVal("HREF", href);

		insertQueryResult(href, title, wtext, bSpan);
	}
	*/

}


void CSearchEngine::ProcessExciteCom(CPageTagNode *document)
{
	// To get the links and snuppets from excite.com search
	// result:
	// Go to body at first, then search first FONT
	// and each p or blockquote is an entry
	CPageTagNode *body, *content, *font;
	CString wtext;
	CString wname;
	CString title;
	CString snippets;
	CString href;
	CString wtemp;
	int indx;

	if((body = document->FindBody()) == NULL)
		return;

	// locate the first FONT tag
	for (content = body->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
			if(stricmp(wname, "font") == 0)
				break;
	}

	if(content == NULL)
		return;

	font = content;
	wtext.Empty();
	font->getText(wtext);
	while(!wtext.IsEmpty())
	{
		indx = wtext.Find("\r\n");
		title = wtext.Left(indx);

		wtemp = wtext.Right(wtext.GetLength()-indx-2);
		wtext = wtemp;

		indx = wtext.Find("\r\n");
		snippets = wtext.Left(indx);

		wtemp = wtext.Right(wtext.GetLength()-indx-2);
		wtext = wtemp;

		indx = wtext.Find("\r\n");
		if(indx == -1)
		{
			href = wtext;
			wtext.Empty();
		}
		else
		{
			href = wtext.Left(indx);

			wtemp = wtext.Right(wtext.GetLength()-indx-2);
			wtext = wtemp;

			wtemp = wtext.Right(wtext.GetLength()-2);
			wtext = wtemp;
		}

		insertQueryResult(href, title, snippets);
	}
}



void CSearchEngine::ProcessWeatherYahoo(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *centre, *high, *low;

	CPageTagNode *content, *font, *ul, *atag;

	CString title;
	CString href;
	CString wname;
	CString wtext;

	wtext.Empty();

	if((body = document->FindBody()) == NULL)
		return;

	centre = body->getFirst("center", 1);
	if(!centre)
		return;

	// point to the last "TABLE" tag
	high = centre->getLast("table");

	low = high->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getLast(); // "TD"
	high = low->getLast(); // "TABLE"
	low = high->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getLast(); // "TD"

	font = low->getLast();
	ul = font->getLast("ul");

	if(!ul)
		return;

	for (content = ul->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "li") == 0)
			{
				atag = content->getFirst("a");

        		title.Empty();
		        atag->getText(title);

        		href.Empty();
        		atag->getAttrVal("HREF", href);

				insertQueryResult(href, title, wtext);
			}
		}
	}
}


void CSearchEngine::ProcessHotbotLycos(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *form, *link, *high, *low;

	CPageTagNode *content, *tbody, *font, *atag;
	int indx;

	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	form = body->getLast();

	// point to the "TABLE" tag
    link = form->getFirst("link", 1);
    if(!link)
        return;

	high = link->getPrev();
	
	low = high->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getLast(); // "TD"

	high = low->getLast(); // "TABLE"
	low = high->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getFirst("td"); // "TD"
	high = low->getFirst("p"); // first "P"

	font = high->getLast();

	for (content = font->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "table") == 0)
			{
				// extract the snippets
				content->getText(snippets);
				indx = snippets.Find("\r\n");
				wtext = snippets.Right(snippets.GetLength()-indx-2);
				indx = wtext.Find("\r\n");
				snippets = wtext.Left(indx);

				tbody = content->getLast();

				high = tbody->getContent(); // "TR"
				low = high->getLast(); // "TD"
				high = low->getLast(); // "FONT"

				atag = high->getFirst("a");

        		title.Empty();
		        atag->getText(title);

        		href.Empty();
        		atag->getAttrVal("HREF", href);

				insertQueryResult(href, title, snippets);
			}
		}
	}
}



void CSearchEngine::ProcessSearchYahoo(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *table6, *high, *low;

	CPageTagNode *content, *ol, *font, *big, *atag;
	int indx;

	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	// point to the sixth "TABLE" tag
	table6 = body->getFirst("table", 6);
	if(table6)
	{
		low = table6->getLast(); // "TBODY"
		high = low->getLast(); // "TR"
		low = high->getContent(); // "TD"

		font = low->getFirst("font");
		if(font == NULL)
			return;
	
		ol = font->getLast();
		if(ol == NULL)
			return;

		snippets.Empty();

		for (content = ol->getContent(); content != NULL; content = content->getNext())
		{
			if(content->getName(wname))
			{
				if(stricmp(wname, "li") == 0)
				{
					// extract the snippets
					content->getText(wtext);
					indx = wtext.Find("\r\n");
					snippets = wtext.Left(indx);

					big = content->getContent();
					atag = big->getContent();

	        		title.Empty();
			        atag->getText(title);

	        		href.Empty();
		    		atag->getAttrVal("HREF", href);

					insertQueryResult(href, title, snippets);
				}
			}
		}
	}
	else
	{
		CPageTagNode *table4, *tbody, *tr, *td2, *div, *span2;
		CString strBuffer;

		table4 = body->getFirst("table", 4);
		if(table4 == NULL)
			return;

		tbody = table4->getContent();
		tr = tbody->getContent();
		td2 = tr->getFirst("td", 2);
		if(td2 == NULL)
			return;

		div = td2->getContent();
		span2 = div->getFirst("span", 2);
		if(span2 == NULL)
		{
			span2 = div->getFirst("span", 1);
			if(span2 == NULL)
				return;
		}

		span2->getText(strBuffer);

		snippets.Empty();
		for (content = span2->getContent(); content != NULL; content = content->getNext())
		{
			if(content->getName(wname))
			{
				if(stricmp(wname, "big") == 0)
				{
					atag = content->getContent();

	        		title.Empty();
			        atag->getText(title);

	        		strBuffer.Empty();
		    		atag->getAttrVal("HREF", strBuffer);

					int offset = strBuffer.Find("*-");
					href = strBuffer.Right(strBuffer.GetLength() - offset - 2);

					insertQueryResult(href, title, snippets);
				}
			}
		}

	}

}



void CSearchEngine::ProcessGoogleYahoo(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *table6, *high, *low;

	CPageTagNode *content, *ol, *font, *big, *atag;
	int indx;

	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	// point to the sixth "TABLE" tag
	table6 = body->getFirst("table", 6);

	low = table6->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getContent(); // "TD"

	font = low->getFirst("font");

    // if there is no search result, then no "font"
    if(font == NULL)
        return;

	ol = font->getLast();

	for (content = ol->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "li") == 0)
			{
				// extract the snippets
				content->getText(wtext);
				indx = wtext.ReverseFind('\r');
				snippets = wtext.Left(indx);

				high = content->getContent(); //"P"
				low = high->getLast();        //"FONT"

				big = low->getContent();
				atag = big->getContent();

        		title.Empty();
		        atag->getText(title);

        		href.Empty();
        		atag->getAttrVal("HREF", href);

				insertQueryResult(href, title, snippets);
			}
		}
	}
}




void CSearchEngine::ProcessSearchMsn(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *table3, *high, *low;

	CPageTagNode *content, *ol, *atag;
	int indx;

	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	// point to the third "TABLE" tag
	table3 = body->getFirst("table", 3);

	low = table3->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getContent(); // "TD"
	high = low->getLast(); // "TABLE"
	low = high->getLast(); // "TBODY"
	high = low->getFirst("tr", 3); // Third "TR"
	low = high->getContent(); // "TD"
	high = low->getFirst("table", 2); // second "TABLE"
	low = high->getLast(); // "TBODY"
	high = low->getLast(); // "TR"
	low = high->getContent(); // "TD"

	ol = low->getFirst("OL", 2); // second "OL"

	for (content = ol->getContent(); content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "li") == 0)
			{
				// extract the snippets
				content->getText(wtext);
				indx = wtext.Find("\r\n");
				snippets = wtext.Left(indx);

				atag = content->getContent();

        		title.Empty();
		        atag->getText(title);

        		href.Empty();
        		atag->getAttrVal("HREF", href);

				insertQueryResult(href, title, snippets);
			}
		}
	}
}




void CSearchEngine::ProcessAtomzCom(CPageTagNode *document)
{
	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	CPageTagNode *body, *p3, *high;

	CPageTagNode *content, *atag;
	int indx;

	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	// point to the third "P" tag
	p3 = body->getFirst("p", 3);
	for (content = p3; content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "p") == 0)
			{
				wtext.Empty();
				if(content->getText(wtext))
				{
					if(wtext.IsEmpty())
						break;

					if((indx = wtext.Find("Sort By Date")) != -1)
						break;

					indx = wtext.ReverseFind('\r');
					snippets = wtext.Left(indx);
				}

				high = content->getContent();
				atag = high->getContent();

        		title.Empty();
		        atag->getText(title);

        		href.Empty();
        		atag->getAttrVal("HREF", href);

				insertQueryResult(href, title, snippets);
			}
		}
	}
}


void CSearchEngine::ProcessAltaVista(CPageTagNode *document)
{
	/*

	// To get the links and snuppets from google.com search
	// result:
	// Go to body at first, then search first div
	// and each p or blockquote is an entry
	HTMLNode *body, *div2;

//	HTMLNode *content;
//	int indx;

//	HTREEITEM prootitem;
	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;

	if((body = document->FindBody()) == NULL)
		return;

	// point to the third "P" tag
	div2 = body->getFirst("div", 2);
	div2->getText(wtext);


	FILE *fp = fopen("test.txt", "wt");
	fprintf(fp, "%s", wtext);
	fclose(fp);

	for (content = p3; content != NULL; content = content->getNext())
	{
		if(content->getName(wname))
		{
			if(stricmp(wname, "p") == 0)
			{
				wtext.Empty();
				if(content->getText(wtext))
				{
					if(wtext.IsEmpty())
						break;

					if((indx = wtext.Find("Sort By Date")) != -1)
						break;

					indx = wtext.ReverseFind('\r\n');
					snippets = wtext.Left(indx);
				}

				high = content->getContent();
				atag = high->getContent();

				title.Empty();
				atag->getText(title);
				prootitem = m_elements.InsertItem(title, TVI_ROOT);

				href.Empty();
				atag->getAttrVal("HREF", href);
				m_elements.InsertItem(href, prootitem);

				m_elements.InsertItem(snippets, prootitem);

			}
		}
	}
	*/
}


void CSearchEngine::ProcessAskJeeves(CPageTagNode *document)
{
	CPageTagNode *pBody, *pDiv, *pDivWebResults, *pWebResults, *pEntry;
	CPageTagNode *pTagA;
	CPageTagNode *pSnippet;
	CPageTagNode *pSpan;

	CString strDivContent;
	CString strBuffer;
	CString wname;

	CString title;
	CString href;
	CString wtext;

	if((pBody = document->FindBody()) == NULL)
		return;

	pDivWebResults = NULL;
	for (pDiv = pBody->getContent(); pDiv != NULL; pDiv = pDiv->getNext())
	{
		if(pDiv->getName(wname) &&  stricmp(wname, "div") == 0)
		{
			strDivContent.Empty();
			pDiv->getText(strDivContent);
			if(!strDivContent.IsEmpty())
			{
				strBuffer = strDivContent.Left(11);
				if(strBuffer.Compare(_T("Web Results")) == 0)
				{
					pDivWebResults = pDiv;
					break;
				}
			}
		}
	}

	if(pDivWebResults)
	{
		pWebResults = pDivWebResults->getFirst("div", 2);
		if(pWebResults)
		{
			pEntry = pWebResults->getContent();
			while (pEntry)
			{
				pDiv = pEntry->getNext();
				pTagA = pDiv->getContent();

				pSnippet = pDiv->getNext();

				pSpan = pSnippet->getNext();

				title.Empty();
				pTagA->getText(title);

				wtext.Empty();
				pSnippet->getText(wtext);

				href.Empty();
				pSpan->getText(href);
				if(!href.IsEmpty())
					href = href.Right(href.GetLength() - 5);

				if(href.IsEmpty())
					pTagA->getAttrVal("HREF", href);

				insertQueryResult(href, title, wtext);

				pEntry = pSpan->getNext();
				pEntry = pEntry->getNext();
			}

		}

	}

}


// This is for WebMind

// recursively process the block quote in google search result page
void CSearchEngine::GoogleSearchResult(CPageTagNode *pContent)
{
	CPageTagNode *content, *ol, *atag, *spantag, *tagTable;
	CPageTagNode *pcontent, *licontent;
	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;
	CString buffer;    
	int pageSize = -1; 

	int slashPosition1;
	int slashPosition2;
    int count=0;
	bool bSpan;
    CString tempTagName=pContent->m_tagName;
    CString tempInnerText=pContent->m_innerText;

	ol=pContent->getContent();
	for (licontent = ol->getContent(); licontent != NULL; licontent = licontent->getNext())
	{
		count++;
		pageSize = -1;
        content=licontent->findSearchItem(licontent);
		if(content==NULL) continue;
	//	content->getName(wname);
	//	if((stricmp("div",wname) != 0 ) && (!content->checkForResultsItem(content)))//if(stricmp(wname, "blockquote") != 0 && stricmp(wname, "p") != 0)
	//		continue;

	/*	if(stricmp(wname, "div") == 0)//if(stricmp(wname, "div") == 0)
		{
//			GoogleSearchResult(content);
//			continue;
			pcontent = content->getContent();
		}
		else*/ //comment by ny;
		pcontent = content;//pcontent = content;

		// whether this is a span entry
		bSpan = false;
		spantag = pcontent->getFirst("span", 1);
		if(spantag)
			bSpan = true;

		// go through all the child nodes, and find the "a" tag
		atag = pcontent->getFirst("a",1);
		//atag= atag->getContent();//atag = pcontent->getFirst("a", 1);
        if(atag == NULL)
            continue;

		// get the title and hyperlink
		title.Empty();
		atag->getText(title);
		href.Empty();
		atag->getAttrVal("HREF", href);

		snippets.Empty();
		pageSize = -1;

		tagTable = pcontent->getFirst("table", 1);
		if(!tagTable)
			tagTable = pcontent->getLast("font");

		if(tagTable)
		{
			CStringArray arSnippets;
			arSnippets.RemoveAll();

			wtext.Empty();
			tagTable->getText(wtext);

			Utility::parseSequence(wtext, _T("\r\n"), arSnippets);
//			for(int i = 0; i < arSnippets.GetSize(); i++)
//				m_LogTrace.WriteLine("%d : %s\n", i, arSnippets[i]);

			if(arSnippets.GetSize())
			{
				// check the last item
				buffer = arSnippets[arSnippets.GetSize() - 1];
				buffer.TrimLeft();
				buffer.TrimRight();

				if(buffer.Find( _T("[ More results from") ) >= 0 ||
					(buffer[0] == '[' && buffer[buffer.GetLength() - 1] == ']') )
					arSnippets.RemoveAt(arSnippets.GetSize() - 1);

				buffer.Empty();
				snippets.Empty();

				if(arSnippets.GetSize() > 0)
					buffer = arSnippets[arSnippets.GetSize() - 1];
				if(arSnippets.GetSize() > 1)
					snippets = arSnippets[arSnippets.GetSize() - 2];

				if(!buffer.IsEmpty())
				{
					slashPosition1 = buffer.Find(" - ", 0) + 3;                                        
					slashPosition2 = buffer.Find(" - ", slashPosition1);                               
					if (slashPosition2 != -1)                                                          
						pageSize = atoi(buffer.Mid(slashPosition1, slashPosition2 - slashPosition1 - 1));
				}
			}
		}

		insertQueryResult(href, title, snippets, bSpan, pageSize);
	}
}


/*
Kevin's program will call this function
void CSearchEngine::GoogleSearchResult(CPageTagNode *pContent)
{
	CPageTagNode *content, *div, *atag, *fonttag, *spantag, *childtag;
	CPageTagNode *pcontent;
	CString title;
	CString snippets;
	CString href;
	CString wtext;
	CString wname;
	CString buffer;    
	int pageSize = -1; 
	int slashPosition1;
	int slashPosition2;
	bool bSpan;

	div = pContent;
	for (content = div->getContent(); content != NULL && (pageSize <= 0 || pageSize > 80); content = content->getNext())
	{
		content->getName(wname);
		if(stricmp(wname, "blockquote") != 0 && stricmp(wname, "p") != 0)
			continue;

		if(stricmp(wname, "blockquote") == 0)
		{
			GoogleSearchResult(content);
			continue;
//			pcontent = content->getContent();
		}
		else
			pcontent = content;

		// div contains a, br and font sections
		atag = pcontent->getContent();
		atag->getName(wname);
        while(atag != NULL && stricmp(wname, "a") != 0)
        {
            atag = atag->getNext();
            if(atag)
       		    atag->getName(wname);
        }

        if(atag == NULL)
            continue;

		// div contains a, br and font sections                   
		fonttag = pcontent->getContent();                         
		fonttag->getName(wname);                                  
        while(fonttag != NULL && stricmp(wname, "font") != 0)
        {                                                    
            fonttag = fonttag->getNext();                    
            if(fonttag)                                      
       		    fonttag->getName(wname);                      
        }                                                    

        if(fonttag == NULL)                                  
            continue;                                        

		// to check whether it is an external file format
		spantag = pcontent->getContent();
		spantag->getName(wname);
        while(spantag != NULL && stricmp(wname, "span") != 0)
        {
            spantag = spantag->getNext();
            if(spantag)
       		    spantag->getName(wname);
        }
		bSpan = false;
		if(spantag)
			bSpan = true;

		title.Empty();
		atag->getText(title);

		wtext.Empty();
		pcontent->getText(wtext);

		childtag = fonttag->getFirst("font", 1);                                             
		if(childtag)                                                                         
		{                                                                                    
			childtag->getText(buffer);                                                         

			slashPosition1 = buffer.Find(" - ", 0) + 3;                                        
			slashPosition2 = buffer.Find(" - ", slashPosition1);                               
			if (slashPosition2 != -1)                                                          
				pageSize = atoi(buffer.Mid(slashPosition1, slashPosition2 - slashPosition1 - 1));
		}                                                                                    

		href.Empty();
		atag->getAttrVal("HREF", href);

		if (!(pageSize == 0 || pageSize > 80))
			insertQueryResult(href, title, wtext, bSpan);
	}
}

*/