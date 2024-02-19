<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- UTF-8 -->

<!--===================== Stylesheet for transforming MatPlot   =====
========================= help source to html for the Html Help =====
=========================        2006 - Matteo Gattanini        ==-->


<xsl:transform version="2.0"
		       xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		       xmlns:mp="/matplot/xhelp">

<!--"ISO-8859-1"  "UTF-8"-->
<xsl:output method="html"
	        media-type="screen"
	        encoding="ISO-8859-1"
	        indent="no"
	        doctype-public="-//W3C//DTD HTML 4.0 Strict//EN"
	        omit-xml-declaration="yes"/>



<!--
=================================== Root template
-->
<xsl:template match="/">
    <html lang="en">
    <xsl:text>&#010;&#013;</xsl:text>
    <xsl:comment>File created with a XSLT transformation: do not edit!</xsl:comment>
    <xsl:text>&#010;&#013;</xsl:text>
    <xsl:comment>(2006 Matteo Gattanini)</xsl:comment>
    <xsl:text>&#010;&#013;</xsl:text>
    <xsl:apply-templates/>
    </html>
</xsl:template>




<!--
=================================== `mp:helptopic' (main tag)
-->
<xsl:template match="mp:helptopic">
    <head>
        <title><xsl:value-of select="@title"/></title>
        <meta name="keywords" content="{@keywords}"/>
        <meta name="Description" content="Html help topic file"/>
        <!-- Link to `Cascading Style Sheet' file -->
        <link rel="StyleSheet" href="./files/hlp_style.css" type="text/css"/>
    </head>
    <body>
        <!-- Link to script file -->
        <script language="Javascript" type="text/javascript" src="./files/hlp_scripts.js"><xsl:text> </xsl:text></script>
        <!-- Build page table structure -->
        <xsl:call-template name="build-pad"/>
    </body>
</xsl:template>


<!--
=================================== `mp:scraptopic' (scrap main tag)
-->
<xsl:template match="mp:scraptopic">
    <head>
        <title><xsl:value-of select="@title"/></title>
        <meta name="keywords" content="{@keywords}"/>
        <meta name="Description" content="Html help topic file"/>
        <!-- Link to `Cascading Style Sheet' file -->
        <link rel="StyleSheet" href="./files/hlp_style.css" type="text/css"/>
    </head>
    <body>
        <xsl:call-template name="build-scrap"/>
    </body>
</xsl:template>




<!--
=================================== `mp:intro' (helptopic intro)
-->
<xsl:template match="mp:helptopic/mp:intro">
    <div class="intro">
        <xsl:apply-templates/>
    </div>
    <xsl:if test="@index='yes'">
        <!-- recursive call to print remaining asterisks -->
        <xsl:call-template name="print-index">
            <xsl:with-param name="verbose" select="1"/>
        </xsl:call-template>
    </xsl:if>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `print-index' (print sections list)
-->
<xsl:template name="print-index">
    <xsl:param name="verbose"/>
    <xsl:text>In this chapter you can read about:</xsl:text>
    <xsl:for-each select="/mp:helptopic/mp:section">
        <ul class="listQ">
            <li><a href="#{@name}"><xsl:value-of select="@title"/></a>
            <xsl:if test="$verbose=1">
                <div class="descriptions">
                    <xsl:value-of select="@description"/>
                </div>
            </xsl:if>
            </li>
        </ul>
    </xsl:for-each>
</xsl:template>





<!--
=================================== Sectioning
-->
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:section' (helptopic section)
-->
<xsl:template match="mp:helptopic/mp:section">
    <!-- Split 
    <object type="application/x-oleobject"
            classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
            <param name="New HTML file" value="{@title}.htm"/>
            <param name="New HTML title" value="{@title}"/>
    </object> -->
    <!-- Write header -->
    <a name="{@name}" title="{@description}"><h2><xsl:value-of select="@title"/></h2></a>
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
</xsl:template>



<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:subsection'
-->
<xsl:template match="mp:subsection">
    <!-- Write header -->
    <a name="{@name}" title="{@keywords}"><h3><xsl:value-of select="@title"/></h3></a>
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:subsubsection'
-->
<xsl:template match="mp:subsubsection">
    <a name="{@name}"><h4><xsl:value-of select="@title"/></h4></a>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
</xsl:template>






<!--
=================================== Some `html' tags, with minor changes
-->


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:br' (break line)
-->
<xsl:template match="mp:br">
    <br/>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:span' (inline text)
-->
<xsl:template match="mp:span">
    <span style="{@style}">
        <xsl:apply-templates/>
    </span>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:p' (paragraph)
-->
<xsl:template match="mp:p">
    <p style="{@style}">
        <xsl:apply-templates/>
    </p>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:a' (link)
-->
<xsl:template match="mp:a" name="link">
    <a>
        <xsl:if test="string(@href)">
        <xsl:attribute name="href">
            <xsl:value-of select="@href"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:if test="string(@onclick)">
        <xsl:attribute name="onclick">
            <xsl:value-of select="@onclick"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:attribute name="title">
        <xsl:choose>
            <xsl:when test="string(@hint)"><xsl:value-of select="@hint"/></xsl:when>
            <xsl:otherwise><xsl:value-of select="@href"/></xsl:otherwise>
        </xsl:choose>
        </xsl:attribute>
        <xsl:apply-templates/>
    </a>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:dfn' (definitions)
-->
<xsl:template match="mp:dfn">
    <dfn title="{@hint}">
        <xsl:apply-templates/>
    </dfn>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:var' (variables-files)
-->
<xsl:template match="mp:var">
    <var title="{@hint}">
        <xsl:apply-templates/>
    </var>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:kbd' (keyboard-actions)
-->
<xsl:template match="mp:kbd">
    <kbd title="{@hint}">
        <xsl:apply-templates/>
    </kbd>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:ac' (acronyms)
-->
<xsl:template match="mp:ac">
    <acronym title="{@hint}">
        <xsl:apply-templates/>
    </acronym>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:cite' (names)
-->
<xsl:template match="mp:cite">
    <xsl:choose>
        <xsl:when test="string(@href)">
            <xsl:call-template name="link"/>
        </xsl:when>
        <xsl:otherwise>
            <cite>
                <xsl:apply-templates/>
            </cite>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:ex' (inline example)
-->
<xsl:template match="mp:ex">
    <samp>
        <xsl:apply-templates/>
    </samp>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:code' (inline code)
-->
<xsl:template match="mp:code">
    <code>
        <xsl:if test="string(@class)">
        <xsl:attribute name="class">
            <xsl:value-of select="@class"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:apply-templates/>
    </code>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:verbatim' (block verbatim)
-->
<xsl:template match="mp:verbatim">
    <pre>
        <xsl:if test="string(@class)">
        <xsl:attribute name="class">
            <xsl:value-of select="@class"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:apply-templates/>
    </pre>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:div' (generic block)
-->
<xsl:template match="mp:div">
    <div>
        <xsl:if test="string(@class)">
        <xsl:attribute name="class">
            <xsl:value-of select="@class"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:if test="string(@style)">
        <xsl:attribute name="style">
            <xsl:value-of select="@style"/>
        </xsl:attribute>
        </xsl:if>
        <xsl:apply-templates/>
    </div>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:img' (images)
-->
<xsl:template match="mp:img">
    <img src="{@src}" title="{@hint}">
            <xsl:if test="string(@class)">
            <xsl:attribute name="class">
                <xsl:value-of select="@class"/>
            </xsl:attribute>
        </xsl:if>
    </img>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:table' (tables)
-->
<xsl:template match="mp:table">
    <table>
        <xsl:if test="string(@class)">
        <xsl:attribute name="class">
            <xsl:value-of select="@class"/>
        </xsl:attribute>
        </xsl:if>
        <!--  Gestione header  -->
        <xsl:for-each select="./mp:thead">
            <thead>
                <xsl:if test="string(@class)">
                <xsl:attribute name="class">
                    <xsl:value-of select="@class"/>
                </xsl:attribute>
                </xsl:if>
                <tr><xsl:for-each select="./mp:c"><td><xsl:apply-templates/></td></xsl:for-each></tr>
            </thead>
        </xsl:for-each>
        <!--  Corpo  -->
        <tbody> 
        <xsl:for-each select="./mp:row">
            <tr>
                <xsl:for-each select="./mp:c">
                    <td>
                        <!--  Gestione colonna header  -->
                        <xsl:if test="position()=1">
                            <xsl:attribute name="style">
                                <xsl:value-of select="@hcol"/>
                            </xsl:attribute>
                        </xsl:if>
                        <!--  Gestione classe  -->
                        <xsl:if test="string(@class)">
                            <xsl:attribute name="class">
                                <xsl:value-of select="@class"/>
                            </xsl:attribute>
                        </xsl:if>
                        <!--  Gestione colspan  -->
                        <xsl:if test="string(@colspan)">
                            <xsl:attribute name="colspan">
                                <xsl:value-of select="@colspan"/>
                            </xsl:attribute>
                        </xsl:if>
                        <xsl:apply-templates/>
                    </td>
                </xsl:for-each>
            </tr>
        </xsl:for-each>
        </tbody> 
        <!--  Gestione footer  -->
        <xsl:for-each select="./mp:tfoot">
            <tfoot>
                <xsl:if test="string(@class)">
                <xsl:attribute name="class">
                    <xsl:value-of select="@class"/>
                </xsl:attribute>
                </xsl:if>
                <tr><xsl:for-each select="./mp:c"><td><xsl:apply-templates/></td></xsl:for-each></tr>
            </tfoot>
        </xsl:for-each>
    </table>
</xsl:template>



<!--
=================================== Added  tags
-->

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:var' (variables-files)
-->
<xsl:template match="mp:file">
        <a href="{@href}" title="{@href}"><var><xsl:value-of select="@name"/></var></a>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:txt' (inline plain text)
-->
<xsl:template match="mp:txt">
    <code class="txt">
        <xsl:apply-templates/>
    </code>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:txtfile' (block verbatim text file)
-->
<xsl:template match="mp:txtfile">
    <!-- Assign a default title if not present -->
    <xsl:variable name="title">
        <xsl:choose>
            <xsl:when test="string(@title)"><xsl:value-of select="@title"/></xsl:when>
            <xsl:otherwise>Notepad</xsl:otherwise>
        </xsl:choose>
    </xsl:variable>
    <table class="txt">
    <thead class="txt"><tr><td><xsl:value-of select="$title"/></td></tr></thead>
    <tbody><tr><td class="txt">
        <pre class="txt" style="{@style}">
            <xsl:apply-templates/>
        </pre>
    </td></tr>
    </tbody></table>
</xsl:template>



<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:cpp' (inline cpp code)
-->
<xsl:template match="mp:cpp">
    <code class="cpp">
        <xsl:apply-templates/>
    </code>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:cppsource' (block verbatim C++ source)
-->
<xsl:template match="mp:cppsource">
    <pre class="cpp" style="{@style}">
        <xsl:apply-templates/>
    </pre>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:cmdline' (block verbatim command-line)
-->
<xsl:template match="mp:cmdline">
    <pre class="cmdline" style="{@style}">
        <xsl:apply-templates/>
    </pre>
</xsl:template>


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:foil' (a showing foil)
-->
<xsl:template match="mp:foil">
    <div class="foil" id="{@id}" style="{@style} display:'none';"> <!-- onclick="noSection({@id})" -->
        <xsl:apply-templates/>
    </div>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:pad' (a pad)
-->
<xsl:template match="mp:pad">
        <xsl:call-template name="build-pad"/>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:scrap' (a scrap)
-->
<xsl:template match="mp:scrap">
        <xsl:call-template name="build-scrap"/>
</xsl:template>


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:sourcefiles' (a list of files)
-->
<xsl:template match="mp:sourcefiles">
    <xsl:for-each select="./mp:file">
        <a href="{@href}" title="{@href}"><var><xsl:value-of select="@name"/></var></a>
        <xsl:if test="position()!=last()"><xsl:text>, </xsl:text></xsl:if>
    </xsl:for-each>
</xsl:template>


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:faqs' (a list of frequently asked questions)
-->
<xsl:template match="mp:faqs">
    <!-- Index -->
    <br/><br/>
    <h3>Index</h3>
    <table class="faqs">
    <tbody>
    <col id="leftcol"/><col id="middlecol"/><col id="rightcol"/>
    <xsl:for-each select="./mp:topic">
        <tr><td class="spacer"></td></tr>
        <!-- Assign a local id if not present -->
        <xsl:variable name="id_top">
            <xsl:choose>
                <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="concat('tp',position())"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <!-- Now write index table -->
        <tr>
            <td colspan="2" style="border:none; font-size:larger;">
            <a name="idx.faq.{$id_top}" title="{@description}" href="#faq.{$id_top}">
                <span class="faqs_topic">
                <xsl:number count="mp:topic"
                        level="single"
                        format="1) "/>
                <xsl:value-of select="@title"/>
                </span>
            </a>
            </td>
            <td></td>
        </tr>
        <xsl:for-each select="./mp:faq">
            <!-- Assign a local id if not present -->
            <xsl:variable name="id_faq">
                <xsl:choose>
                    <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                    <xsl:otherwise><xsl:value-of select="concat('faq',position())"/></xsl:otherwise>
                </xsl:choose>
            </xsl:variable>
            <!-- Now write table -->
            <tr>
                <td></td>
                <td colspan="2" style="border:none; padding:2px;">
                <a name="idx.faq.{$id_top}.{$id_faq}" title="{@keywords}" href="#faq.{$id_top}.{$id_faq}">
                    <span class="faqs_que">
                    <xsl:number count="mp:topic|mp:faq"
                            level="multiple"
                            format="1) "/>
                        <xsl:apply-templates select="./mp:que"/>
                    </span>
                </a>
                </td>
            </tr>
        </xsl:for-each>
    </xsl:for-each>
    </tbody></table>
    <br/><br/>
    <!-- Body <h3>Body of frequently asked questions</h3> -->
    <table class="faqs">
    <tbody>
    <col id="leftcol"/><col id="middlecol"/><col id="rightcol"/>
    <xsl:for-each select="./mp:topic">
        <tr><td class="spacer"></td></tr>
        <!-- Assign a local id if not present -->
        <xsl:variable name="id_top">
            <xsl:choose>
                <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="concat('tp',position())"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <!-- Now write body table -->
        <tr>
            <td class="faqs_topic" colspan="3">
            <a name="faq.{$id_top}" title="{@description}" href="#idx.faq.{$id_top}">
                <span class="faqs_topic">
                <xsl:number count="mp:topic"
                        level="single"
                        format="1) "/>
                <xsl:value-of select="@title"/>
                </span>
            </a>
            </td>
        </tr>
        <xsl:for-each select="./mp:faq">
            <tr><td class="spacer"></td></tr>
            <!-- Manage attribute-passed keywords -->
            <xsl:call-template name="HHmake-keywords">
                <xsl:with-param name="keywordlist" select="@keywords"/>
            </xsl:call-template>
            <!-- Assign a local id if not present -->
            <xsl:variable name="id_faq">
                <xsl:choose>
                <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="concat('faq',position())"/></xsl:otherwise>
                </xsl:choose>
            </xsl:variable>
            <!-- Now write table -->
            <tr>
                <td class="faqs_que" colspan="2">
                <a name="faq.{$id_top}.{$id_faq}" title="{@keywords}" href="#idx.faq.{$id_top}.{$id_faq}">
                    <span class="faqs_que" style="font-size:larger;">
                    <xsl:number count="mp:topic|mp:faq"
                            level="multiple"
                            format="1) "/>
                        <xsl:apply-templates select="./mp:que"/>
                    </span>
                </a>
                </td>
                <td></td>
            </tr>
            <tr>
                <td></td>
                <td class="faqs_ans" colspan="2">
                    <xsl:apply-templates select="./mp:ans"/>
                </td>
            </tr>
        </xsl:for-each>
    </xsl:for-each>
    </tbody></table>
</xsl:template>



<!--
=================================== Lists
-->
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:listQ'
-->
<xsl:template match="mp:listQ">
    <ul class="listQ">
        <xsl:apply-templates/>
    </ul>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:listO'
-->
<xsl:template match="mp:listO">
    <ul class="listO">
        <xsl:apply-templates/> 
    </ul>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:listD'
-->
<xsl:template match="mp:listD">
    <ul class="listD">
        <xsl:apply-templates/>
    </ul>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:it' (a list item)
-->
<xsl:template match="mp:it">
    <li>
        <xsl:apply-templates/>
    </li>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:ds' (an item description)
-->
<xsl:template match="mp:it/mp:ds">
    <div class="descriptions">
        <xsl:apply-templates/>
    </div>
</xsl:template>




<!--
=================================== Utilities
-->

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:underconstruction' (mark a not ready zone)
-->
<xsl:template match="mp:underconstruction">
    <img src="./files/images/underconstruction.gif" style="float:right;margin:1em;"/>
</xsl:template>






<!--
=================================== `Html help' objects
-->

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHobject' HH generic activeX object instance
-->
<xsl:template match="mp:HHobject">
    <object id="{@id}" type="application/x-oleobject"
        classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11">
    </object>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHbutton' (a button)
-->
<xsl:template match="mp:HHbutton">
    <object id="{@id}" type="application/x-oleobject"
        classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"
        codebase="hhctrl.ocx#Version=5,2,3790,1194"
        width="100" height="100">
        <param name="Button" value="{@caption}"/>
        <param name="Command" value="{@command}"/>
    </object>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHseealso' (button of related topics)
-->
<xsl:template match="mp:HHseealso">
    <div style="text-align:right;">
    <object id="{@id}" type="application/x-oleobject"
        classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"
        codebase="hhctrl.ocx#Version=5,2,3790,1194"
        width="100" height="100">
        <param name="Button" value="Text: see also"/> 
        <param name="Command" value="Related Topics,menu"/>
        <xsl:for-each select="./mp:target">
            <xsl:variable name="paramname" select="concat('Item',position())"/>
            <param name="{$paramname}" value="{@title};{@href}"/>
        </xsl:for-each>
    </object>
    </div>
</xsl:template>



<!--
=================================== Keywords
-->

<!--
+++++++++++++++++++++++++++++++++++ `HHmake-keywords' (make HH keywords from attribute)
-->
<xsl:template name="HHmake-keywords">
    <xsl:param name="keywordlist"/>
    <xsl:if test="string-length($keywordlist)>0">
        <object type="application/x-oleobject"
            classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
            <xsl:variable name="adaptedlist" select="concat(normalize-space($keywordlist),',')"/>
            <xsl:call-template name="HHmake-csv-keywords">
                <xsl:with-param name="cslist" select="$adaptedlist"/>
            </xsl:call-template>
        </object>
    </xsl:if>
</xsl:template>
<!--
+++++++++++++++++++++++++++++++++++ Make comma separated keywords
-->
<xsl:template name="HHmake-csv-keywords">
    <xsl:param name="cslist"/>
    <xsl:variable name="adaptedlist" select="normalize-space($cslist)"/>
    <xsl:if test="string-length($adaptedlist)>2">
        <xsl:variable name="first" select="substring-before($adaptedlist,',')"/>
        <!-- Add keyword -->
        <param name="Keyword" value="{normalize-space($first)}"/>
        <!-- Recursion -->
        <xsl:variable name="rest" select="substring-after($adaptedlist,',')"/>
        <xsl:call-template name="HHmake-csv-keywords">
            <xsl:with-param name="cslist" select="$rest"/>
        </xsl:call-template>
    </xsl:if>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHkeywords,mp:keyword' (make HH keywords from tags)
-->
<xsl:template match="mp:HHkeywords">
    <object type="application/x-oleobject"
        classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
        <xsl:apply-templates/>
    </object>
</xsl:template>
<xsl:template match="mp:HHkeywords/mp:keyword">
    <param name="Keyword" value="{@value}"/>
</xsl:template>



<!--
=================================== Functions
-->

<!--
+++++++++++++++++++++++++++++++++++ `build-pad' (build a pad page)
-->
<xsl:template name="build-pad">
    <!-- Page table structure -->
    <table class="page" border="0">
    <!-- Page Header -->
    <thead class="page"><tr>
    <td class="pad_TL">
        <xsl:if test="string-length(@prev)>0">
        <a class="browse" href="{@prev}.htm"><img class="browse" src="./files/images/Browse_Prev.gif" alt="Previous"/></a>
        </xsl:if>
    </td>
    <td class="pad_T">
        <xsl:if test="string-length(@up)>0">
        <a class="browse" href="{@up}.htm" name="top_page"><img class="browse" src="./files/images/Browse_Top.gif" alt="Up"/></a>
        </xsl:if>
    
    </td>
    <td class="pad_TR">
        <xsl:if test="string-length(@next)>0">
        <a class="browse" href="{@next}.htm"><img class="browse" src="./files/images/Browse_Next.gif" alt="Next"/></a>
        </xsl:if>
    </td>
    </tr></thead>
    <!-- Page Footer -->
    <tfoot class="page"><tr>
    <td class="pad_BL">
        <xsl:if test="string-length(@prev)>0">
        <a class="browse" href="{@prev}.htm"><img class="browse" src="./files/images/Browse_Prev.gif" alt="Previous"/></a>
        </xsl:if>
    </td>
    <td class="pad_B">
        <xsl:if test="string-length(@up)>0">
        <a class="browse" href="#top_page"> <img class="browse" src="./files/images/Browse_Top.gif" alt="Top of page"/></a>
        </xsl:if>
    </td>
    <td class="pad_BR">
        <xsl:if test="string-length(@next)>0">
        <a class="browse" href="{@next}.htm"><img class="browse" src="./files/images/Browse_Next.gif" alt="Next"/></a>
        </xsl:if>
    </td>
    </tr></tfoot>
    <!-- Start Page Body -->
    <tbody><tr><td class="pad_L"/><td class="pad_Body">
    <!-- Version -->
    <div class="docversion"><xsl:value-of select="@version"/><xsl:text> , </xsl:text><xsl:value-of select="@date"/></div>
    <!-- Write title header -->
    <a name="{@name}" title="{@description}"><h1><xsl:value-of select="@title"/></h1></a>
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
    <!-- End Page Body -->
    </td><td class="pad_R"/></tr></tbody></table>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `build-scrap' (build a scrap foil)
-->
<xsl:template name="build-scrap">
    <!-- Page table structure -->
    <table class="page" border="0">
        <xsl:if test="string(@id)">
        <xsl:attribute name="id">
            <xsl:value-of select="@id"/>
        </xsl:attribute>
        </xsl:if>
    <!-- Page Header -->
    <thead class="page">
    <tr><td class="scrap_TL"></td><td class="scrap_T"></td><td class="scrap_TR"></td></tr></thead>
    <!-- Page Footer -->
    <tfoot class="page">
    <tr><td class="scrap_BL"></td><td class="scrap_B"></td><td class="scrap_BR"></td></tr></tfoot>  
    <!-- Start Scrap Body -->
    <tbody><tr><td class="scrap_L"/><td class="scrap_Body">
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
    <!-- End Scrap Body -->
    </td><td class="scrap_R"/></tr>
    </tbody></table>
</xsl:template>


<!--
+++++++++++++++++++++++++++++++++++ Sum space-separated numbers like '67.2 12  56 3'
-->
<xsl:template name="sum-spaced-numbers">
    <xsl:param name="numlist"/>
    <xsl:variable name="list" select="concat(normalize-space($numlist),' ')"/>
    <xsl:choose>
        <xsl:when test="$list!=' '">
            <xsl:variable name="first" select="substring-before($list,' ')"/>
            <xsl:variable name="rest" select="substring-after($list,' ')"/>
            <!-- Recursion -->
            <xsl:variable name="total">
                <xsl:call-template name="sum-spaced-numbers">
                    <xsl:with-param name="numlist" select="$rest"/>
                </xsl:call-template>
            </xsl:variable>
            <!-- Compute partial sum -->
            <xsl:value-of select="number($first) + number($total)"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
</xsl:template>




</xsl:transform> <!--================ End of File ================-->
