<?xml version="1.0" encoding="windows-1252"?>
<!--===================== Stylesheet for transforming MattPlot  =====
========================= help source to html for the Html Help =====
=========================        2009 - Matteo Gattanini        ==-->

<!DOCTYPE transform
    [
    <!ENTITY dummy "<xsl:text>&#160;</xsl:text>">
    <!ENTITY nbsp "<xsl:text>&#160;</xsl:text>">
    ]>

<xsl:transform version="2.0"
               xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               xmlns:fn="http://www.w3.org/2005/xpath-functions"
               xmlns:mp="/mattplot/xhelp"
               xmlns:saxon="http://saxon.sf.net/"
               xmlns="http://www.w3.org/1999/xhtml"
               exclude-result-prefixes="mp fn saxon"
               extension-element-prefixes="saxon">
<!-- new namespace: xmlns:saxon="http://saxon.sf.net/" -->
<!-- old namespace: xmlns:saxon="http://icl.com/saxon" -->
<!-- <xsl:import href="extern.xsl"/> -->

<!-- Global variables -->
<xsl:variable name="out_htm_dir" select="'./chm/html/'"/>
<xsl:variable name="out_ext" select="'html'"/>
<xsl:variable name="use_chmobj" select="0"/>
<xsl:variable name="cdate" select="fn:current-date()"/>

<!--"ISO-8859-1"  "UTF-8"  "windows-1252"-->
<xsl:output method="xhtml"
            media-type="screen"
            encoding="ISO-8859-1"
            indent="yes"
            doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"
            doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
            omit-xml-declaration="yes"/>

<!--
            doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"
            doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"

            doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
            doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"

            doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
            doctype-public="-//W3C//DTD XHTML 1.1//EN"
-->

<!--
=================================== Root template
-->
<xsl:template match="/">
    <!-- The main file produces no output -->
    <!-- TOC file for chm project -->
    <xsl:call-template name="create-chm-toc">
        <xsl:with-param name="path" select="'./chm/TOC.hhc'"/>
    </xsl:call-template>
    <!-- Alias file for chm project -->
    <xsl:call-template name="create-alias-file">
        <xsl:with-param name="path" select="'./chm/hlp_MattPlot.ali'"/>
    </xsl:call-template>
    <!-- Header file for chm project -->
    <xsl:call-template name="create-header-file">
        <xsl:with-param name="path" select="'./chm/hlp_MattPlot.h'"/>
    </xsl:call-template>

    <xsl:apply-templates/>
</xsl:template>


<!--
=================================== `mp:scrap_page' (scrap main tag)
-->
<xsl:template match="mp:scrap_page">
    <xsl:result-document href="{$out_htm_dir}/{@name}.{$out_ext}">
    <html lang="en">
    <xsl:call-template name="print-html-header"/>
    <body>
        <!-- Manage attribute-passed keywords -->
        <xsl:call-template name="HHmake-keywords">
            <xsl:with-param name="keywordlist" select="@keywords"/>
        </xsl:call-template>
        <!-- Build scrap body -->
        <xsl:call-template name="build-scrap"/>
    </body>
    </html>
    </xsl:result-document>
</xsl:template>



<!--
=================================== Sectioning
-->

<!--
=================================== `mp:chapter'
-->
<xsl:template match="mp:chapter">
    <xsl:result-document href="{$out_htm_dir}/{@name}.{$out_ext}">
    <!-- Split 
    <object type="application/x-oleobject"
            classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
            <param name="New HTML file" value="{@title}.{$out_ext}"/>
            <param name="New HTML title" value="{@title}"/>
    </object> -->
    <html>
      <xsl:call-template name="print-html-header"/>
      <body>
          <!-- Build page table structure -->
          <xsl:call-template name="build-pad">
              <xsl:with-param name="level" select="1"/>
          </xsl:call-template>
      </body>
    </html>
    </xsl:result-document>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:section' (chapter section)
-->
<xsl:template match="mp:chapter/mp:section">
    <xsl:result-document href="{$out_htm_dir}/{@name}.{$out_ext}">
    <html>
      <xsl:call-template name="print-html-header"/>
      <body>
          <!-- Build page table structure -->
          <xsl:call-template name="build-pad">
              <xsl:with-param name="level" select="2"/>
          </xsl:call-template>
      </body>
    </html>
    </xsl:result-document>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:subsection'
-->
<xsl:template match="mp:section/mp:subsection-rich">
    <xsl:result-document href="{$out_htm_dir}/{@name}.{$out_ext}">
    <html>
      <xsl:call-template name="print-html-header"/>
      <body>
          <!-- Build page table structure -->
          <xsl:call-template name="build-pad">
              <xsl:with-param name="level" select="3"/>
          </xsl:call-template>
      </body>
    </html>
    </xsl:result-document>
</xsl:template>

<xsl:template match="mp:section/mp:subsection">
    <!-- Write header -->
    <h3><a name="{@name}" title="{@keywords}"><xsl:value-of select="@title"/></a></h3>
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
    <h4><a name="{@name}"><xsl:value-of select="@title"/></a></h4>
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
</xsl:template>



<!--
=================================== Some facilities
-->

<!--
=================================== `mp:intro' (chapter intro)
-->
<xsl:template match="mp:chapter/mp:intro">
    <div class="intro">
        <xsl:apply-templates/>
    </div>
    <xsl:if test="@toc='yes'">
        <!-- print chapter contents -->
        <xsl:call-template name="print-contents-toc">
            <xsl:with-param name="sectnodes" select="'../mp:section'"/>
            <xsl:with-param name="verbose" select="'yes'"/>
        </xsl:call-template>
    </xsl:if>
</xsl:template>

<!--
=================================== `mp:print-toc'
-->
<xsl:template match="mp:print-toc">
    <!-- print help contents -->
    <xsl:call-template name="print-help-toc">
        <xsl:with-param name="verbose" select="@verbose"/>
    </xsl:call-template>
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:em' (emphasis)
-->
<xsl:template match="mp:em">
    <em><xsl:apply-templates/></em>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:strong' (strong emphasis)
-->
<xsl:template match="mp:strong">
    <strong><xsl:apply-templates/></strong>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:sup' (exponent)
-->
<xsl:template match="mp:sup">
    <sup>
        <xsl:apply-templates/>
    </sup>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:p' (paragraph)
-->
<xsl:template match="mp:p">
    <p>
      <xsl:if test="fn:string-length(@style)&gt;0">
        <xsl:attribute name="style"><xsl:value-of select="@style"/></xsl:attribute>
      </xsl:if>
      <xsl:apply-templates/>
    </p>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:a' (link)
-->
<xsl:template match="mp:a" name="link">
    <a>
      <xsl:if test="string(@href)">
        <xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
      </xsl:if>
      <xsl:if test="string(@name)">
        <xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
      </xsl:if>
      <xsl:if test="string(@onclick)">
        <xsl:attribute name="onclick"><xsl:value-of select="@onclick"/></xsl:attribute>
      </xsl:if>
      <xsl:attribute name="title">
          <xsl:choose>
              <xsl:when test="string(@href)"><xsl:value-of select="@href"/></xsl:when>
              <xsl:when test="string(@hint)"><xsl:value-of select="@hint"/></xsl:when>
              <xsl:otherwise><xsl:value-of select="@name"/></xsl:otherwise>
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:var' (variables)
-->
<xsl:template match="mp:var">
    <var title="{@hint}">
        <xsl:apply-templates/>
    </var>
</xsl:template>
<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:dir' (directories,files)
-->
<xsl:template match="mp:dir">
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:quote' (quotation, emphasized text)
-->
<xsl:template match="mp:quote">
    <q title="{@hint}">
        <xsl:apply-templates/>
    </q>
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
  <img src="{@src}" title="{@hint}" style="{@style}">
    <xsl:if test="fn:string-length(@class)&gt;0">
      <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
    </xsl:if>
    <xsl:if test="fn:string-length(@alt)&lt;1">
      <xsl:attribute name="alt">[<xsl:value-of select="@src"/>]</xsl:attribute>
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
                        <xsl:if test="position()=1 and ../../@hcol=1">
                            <xsl:attribute name="class">
                                <xsl:value-of select="'header'"/>
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:file' (variables-files)
-->
<xsl:template match="mp:file">
    <xsl:choose>
       <xsl:when test="string(@href)">
         <a href="{@href}" title="{@href}"><dir><xsl:value-of select="@name"/></dir></a>
       </xsl:when>
       <xsl:otherwise>
         <a href="{@name}" title="{@name}"><dir><xsl:value-of select="@name"/></dir></a>
       </xsl:otherwise>
    </xsl:choose>
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:notepad' (block verbatim text file)
-->
<xsl:template match="mp:notepad" name="build-notepad">
    <!-- Assign a default title if not present -->
    <xsl:variable name="title">
        <xsl:choose>
            <xsl:when test="fn:string-length(@caption)&gt;0"><xsl:value-of select="@caption"/></xsl:when>
            <xsl:otherwise>Notepad</xsl:otherwise>
        </xsl:choose>
    </xsl:variable>
    <div class="notepad">&dummy;<xsl:value-of select="$title"/>
      <pre class="notepad">
          <xsl:apply-templates/>
      </pre>
    </div>
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
    <xsl:call-template name="build-notepad"/>
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
  <div style="width: 80%; margin: auto;">
    <xsl:call-template name="build-scrap"/>
  </div>
</xsl:template>


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:sourcefiles' (a list of files)
-->
<xsl:template match="mp:sourcefiles">
    <xsl:for-each select="./mp:file">
        <a href="{@href}" title="{@href}"><dir><xsl:value-of select="@name"/></dir></a>
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
    <col class="leftcol"/><col class="middlecol"/><col class="rightcol"/>
    <tbody>
    <xsl:for-each select="./mp:topic">
        <tr><td class="spacer"></td></tr>
        <!-- Assign a local id if not present -->
        <xsl:variable name="id_tpc">
            <xsl:choose>
                <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="concat('tpc_',position())"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <!-- Now write index table -->
        <tr>
            <td colspan="2" style="border:none; font-size:larger;">
            <a name="idx.faq.{$id_tpc}" title="{@description}" href="#faq.{$id_tpc}">
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
                    <xsl:otherwise><xsl:value-of select="concat('faq_',position())"/></xsl:otherwise>
                </xsl:choose>
            </xsl:variable>
            <!-- Now write table -->
            <tr>
                <td></td>
                <td colspan="2" class="faqs_que">
                  <a name="idx.faq.{$id_tpc}.{$id_faq}" title="{@keywords}" href="#faq.{$id_tpc}.{$id_faq}">
                    <xsl:number count="mp:topic|mp:faq"
                                level="multiple"
                                format="1) "/>
                  </a>
                  <xsl:apply-templates select="./mp:que"/>
                </td>
            </tr>
        </xsl:for-each>
    </xsl:for-each>
    </tbody></table>
    <br/><br/>
    <!-- Body <h3>Body of frequently asked questions</h3> -->
    <table class="faqs">
    <col class="leftcol"/><col class="middlecol"/><col class="rightcol"/>
    <tbody>
    <xsl:for-each select="./mp:topic">
        <tr><td class="spacer"></td></tr>
        <!-- Assign a local id if not present -->
        <xsl:variable name="id_tpc">
            <xsl:choose>
                <xsl:when test="string(@name)"><xsl:value-of select="@name"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="concat('tp',position())"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <!-- Now write body table -->
        <tr>
            <td class="faqs_topic" colspan="3">
            <a name="faq.{$id_tpc}" title="{@description}" href="#idx.faq.{$id_tpc}">
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
            <tr><td class="spacer">
              <!-- Manage attribute-passed keywords -->
              <xsl:call-template name="HHmake-keywords">
                  <xsl:with-param name="keywordlist" select="@keywords"/>
              </xsl:call-template>
            </td></tr>
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
                  <a name="faq.{$id_tpc}.{$id_faq}" title="{@keywords}" href="#idx.faq.{$id_tpc}.{$id_faq}">
                    <xsl:number count="mp:topic|mp:faq"
                            level="multiple"
                            format="1) "/>
                  </a>
                  <xsl:apply-templates select="./mp:que"/>
                </td>
                <td></td>
            </tr>
            <tr>
                <td></td>
                <!--
                <td class="faqs_ans" colspan="2">
                    <xsl:apply-templates select="./mp:ans"/>
                </td> -->
                <td class="faqs_ans" colspan="2">
                    <xsl:call-template name="build-scrap">
                      <xsl:with-param name="node" select="./mp:ans"/>
                    </xsl:call-template>
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:list'
-->
<xsl:template match="mp:list">
    <ul>
        <xsl:apply-templates/>
    </ul>
</xsl:template>
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
    <img src="./files/images/underconstruction.gif" style="float:right;margin:1em;" alt="[Under construction]"/>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:action' (action reference)
-->
<xsl:template match="mp:action">
    <a href="ActionsTable.{$out_ext}#act{@name}" title="action {@name}" style="margin-left:0.5ex;">
        <img src="./files/images/actions/{@name}.gif" alt="[{@name}]"/><xsl:text>&#160;</xsl:text><code class="txt"><xsl:value-of select="@name"/></code>
    </a>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:option' (option reference)
-->
<xsl:template match="mp:option">
    <a href="OptionsDescription.{$out_ext}#opt{@name}" title="option {@name}">
        <code class="txt"><xsl:value-of select="@name"/></code>
    </a>
</xsl:template>




<!--
=================================== `Html help' objects
-->

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHobject' HH generic activeX object instance
-->
<xsl:template match="mp:HHobject">
    <xsl:choose>
        <xsl:when test="$use_chmobj=0">
        </xsl:when>
        <xsl:otherwise>
            <object id="{@id}" type="application/x-oleobject"
                    classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11">
            </object>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHbutton' (a button)
-->
<xsl:template match="mp:HHbutton">
    <xsl:choose>
        <xsl:when test="$use_chmobj=0">
            <p title="{@item1} disabled for compatibility with IE7"
               style="width: auto;
                      float: right;
                      clear: both;"
               class="button">
                <xsl:value-of select="@caption"/>
            </p>
        </xsl:when>
        <xsl:otherwise>
            <object id="{@id}" type="application/x-oleobject"
                    classid="clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11"
                    codebase="hhctrl.ocx#Version=5,2,3790,1194"
                    width="100" height="100">
                <param name="Button" value="{@caption}"/>
                <param name="Command" value="{@command}"/>
                <xsl:if test="string(@item1)">
                    <param name="Item1" value="{@item1}"/>
                </xsl:if>
                <xsl:if test="string(@item2)">
                    <param name="Item2" value="{@item2}"/>
                </xsl:if>
                <xsl:if test="string(@item3)">
                    <param name="Item3" value="{@item3}"/>
                </xsl:if>
            </object>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>


<!--
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ `mp:HHseealso' (button of related topics)
-->
<xsl:template match="mp:HHseealso">
    <div style="width: 25%; float: right; clear: both; border: 1px solid rgb(172,169,154); margin: 1ex; padding: 0px; spacing: 0px;">
    <xsl:choose>
        <xsl:when test="$use_chmobj=0">
            <table style="border-collapse: collapse; border-spacing: 0px; border: none; margin: auto;">
            <caption style="caption-side: top; text-align: left; color: gray;">See also</caption>
            <tbody>
                <xsl:for-each select="./mp:target">
                    <tr><td style="border: none; margin: 0px; padding: 1px; spacing: 0px;">
                      <a class="button" href="{@href}"><xsl:value-of select="@title"/></a>
                    </td></tr>
                </xsl:for-each>
            </tbody>
            </table>
        </xsl:when>
        <xsl:otherwise>
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
        </xsl:otherwise>
    </xsl:choose>
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
+++++++++++++++++++++++++++++++++++ `create-chm-toc'
-->
<xsl:template name="create-chm-toc">
    <xsl:param name="path"/>
    <xsl:result-document method="html" href="{$path}" doctype-public="-//IETF//DTD HTML//EN" indent="yes">
    <!-- <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN"> -->
    <HTML>
    <HEAD>
    <meta name="GENERATOR" content="XSL transformation"></meta>
    <xsl:comment> Sitemap 1.0 </xsl:comment>
    </HEAD><BODY>
    <OBJECT type="text/site properties">
        <param name="Auto Generated" value="Yes"/>
    </OBJECT>
    <xsl:for-each select="//mp:chapter">
        <UL>
            <LI> <OBJECT type="text/sitemap">
                 <param name="Name" value="{@title}"/>
                 <param name="Local" value="html/{@name}.{$out_ext}"/>
                 </OBJECT>
            <UL>
              <xsl:for-each select="./mp:section">
                <LI> <OBJECT type="text/sitemap">
                    <param name="Name" value="{@title}"/>
                    <param name="Local" value="html/{@name}.{$out_ext}"/>
                    </OBJECT>
                </LI>
                <UL>
                  <xsl:for-each select="./mp:subsection">
                    <LI> <OBJECT type="text/sitemap">
                        <param name="Name" value="{@title}"/>
                        <param name="Local" value="html/{../@name}.{$out_ext}#{@name}"/>
                        </OBJECT>
                    </LI>
                  </xsl:for-each>
                </UL>
              </xsl:for-each>
            </UL>
            </LI>
        </UL>
    </xsl:for-each>
    </BODY></HTML>
    </xsl:result-document>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `create-alias-file'
-->
<xsl:template name="create-alias-file">
    <xsl:param name="path"/>
    <xsl:result-document method="text" href="{$path}">
    <xsl:for-each select="//mp:chapter">
      <xsl:text>IDH_</xsl:text><xsl:value-of select="@name"/><xsl:text>=html\</xsl:text><xsl:value-of select="@name"/><xsl:text>.</xsl:text><xsl:value-of select="$out_ext"/><xsl:text> ; </xsl:text><xsl:value-of select="@title"/><xsl:text>&#013;&#010;</xsl:text>
      <xsl:for-each select="./mp:section">
        <xsl:text>IDH_</xsl:text><xsl:value-of select="@name"/><xsl:text>=html\</xsl:text><xsl:value-of select="@name"/><xsl:text>.</xsl:text><xsl:value-of select="$out_ext"/><xsl:text> ; </xsl:text><xsl:value-of select="@title"/><xsl:text>&#013;&#010;</xsl:text>
      </xsl:for-each>
    </xsl:for-each>
    </xsl:result-document>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `create-header-file'
-->
<!--
ehm, cannot use parent::position() or change grouping sep in
<xsl:number grouping-separator="," level="multiple" count="mp:chapter | mp:section" format="1"/>

<xsl:variable name="currchapter" select="0" saxon:assignable="yes"/>
<xsl:template name="create-header-file">
    <xsl:param name="path"/>
    <xsl:variable name="chaptersize" select="number(1000)"/>
    <xsl:result-document method="text" href="{$path}">
    <xsl:text>// Include me in program project to reference help topics</xsl:text><xsl:text>&#013;&#010;</xsl:text>
    <xsl:for-each select="//mp:chapter">
      <saxon:assign name="currchapter" select="$chaptersize*position()"/>
      <xsl:text>#define IDH_</xsl:text><xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="$currchapter"/><xsl:text>&#013;&#010;</xsl:text>
      <xsl:for-each select="./mp:section">
        <xsl:text>#define IDH_</xsl:text><xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="$currchapter + position()"/><xsl:text>&#013;&#010;</xsl:text>
      </xsl:for-each>
    </xsl:for-each>
    </xsl:result-document>
</xsl:template>
-->
<xsl:template name="create-header-file">
    <xsl:param name="path"/>
    <xsl:variable name="chaptersize" select="number(1000)"/>
    <xsl:result-document method="text" href="{$path}">
        <xsl:text>// Include me in program project to reference help topics</xsl:text><xsl:text>&#013;&#010;</xsl:text>
        <xsl:for-each select="//mp:chapter">
          <xsl:text>#define IDH_</xsl:text>
          <xsl:value-of select="@name"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="$chaptersize*position()"/><xsl:text>&#013;&#010;</xsl:text>
            <!-- Do sections -->
              <xsl:call-template name="header-file-section-entry">
                    <xsl:with-param name="chapternum" select="$chaptersize*position()"/>
              </xsl:call-template>
        </xsl:for-each>
    </xsl:result-document>
</xsl:template>
<!-- === -->
<xsl:template name="header-file-section-entry">
    <xsl:param name="chapternum"/>
      <xsl:for-each select="./mp:section">
          <xsl:text>#define IDH_</xsl:text>
           <xsl:value-of select="@name"/>
           <xsl:text> </xsl:text>
           <xsl:value-of select="$chapternum + position()"/>
           <xsl:text>&#013;&#010;</xsl:text>
      </xsl:for-each>
</xsl:template>


<!--
+++++++++++++++++++++++++++++++++++ `print-help-toc'
-->
<xsl:template name="print-help-toc">
    <xsl:param name="verbose"/>
    <xsl:for-each select="//mp:chapter">
        <ul class="listQ" style="font-size:100%;">
            <li><h4><a href="{@name}.{$out_ext}"><xsl:value-of select="@title"/></a></h4>
            <xsl:if test="$verbose='yes'">
                <div class="descriptions" style="padding-left:5em;">
                    <xsl:value-of select="@description"/>
                </div>
            </xsl:if>
            <xsl:for-each select="./mp:section">
                <ul class="listD">
                    <li style="font-size:80%;"><a href="{@name}.{$out_ext}"><xsl:value-of select="@title"/></a>
                    <xsl:if test="$verbose='yes'">
                        <div class="descriptions" style="display:inline;font-size:smaller;">
                            (<xsl:value-of select="@description"/>)
                        </div>
                    </xsl:if>
                    </li>
                </ul>
            </xsl:for-each>
            </li>
        </ul>
    </xsl:for-each>
    <h2>List of Scraps</h2>
    <ul class="listO" style="font-size:80%;">
      <xsl:for-each select="//mp:scrap_page">
         <li><h4><a href="{@name}.{$out_ext}"><xsl:value-of select="@title"/></a></h4></li>
      </xsl:for-each>
    </ul>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `print-contents-toc'
-->
<xsl:template name="print-contents-toc">
    <xsl:param name="sectnodes"/> <!-- sectioning node -->
    <xsl:param name="verbose"/>
        <!-- <xsl:for-each select="saxon:evaluate($sectnodes)"> -->
        <xsl:for-each select="saxon:eval(saxon:expression($sectnodes))">
        <ul class="listQ">
            <li><h4><a href="{@name}.{$out_ext}"><xsl:value-of select="@title"/></a></h4>
            <xsl:if test="$verbose='yes'">
                <div class="descriptions">
                    <xsl:value-of select="@description"/>
                </div>
            </xsl:if>
            </li>
        </ul>
    </xsl:for-each>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `build-tabs' (tabs)
-->
<xsl:template name="build-tabs">
  <xsl:param name="sectnodes"/> <!-- sectioning nodes -->
  <xsl:param name="currsection"/>
  <div class="tabs">
    <ul>
        <!-- <xsl:for-each select="saxon:evaluate($sectnodes)"> -->
        <xsl:for-each select="saxon:eval(saxon:expression($sectnodes))">
        <li>
          <xsl:if test="string(@name)=$currsection">
            <xsl:attribute name="id">current</xsl:attribute>
          </xsl:if>
          <a href="{@name}.{$out_ext}"><span><xsl:value-of select="@title"/></span></a>
          <!-- target="iframe_{../@name}" -->
        </li>
      </xsl:for-each>
    </ul>
  </div>
</xsl:template>


<!--
+++++++++++++++++++++++++++++++++++ `build-pad' (build a pad page)
-->
<xsl:template name="build-pad">
    <xsl:param name="level"/> <!-- sectioning level -->
    <!-- Prepare browsing links -->
    <xsl:variable name="up">
      <xsl:choose>
        <xsl:when test="$level=1"> <!-- chapter -->
          <!-- Avoid up arrow in Default page -->
          <xsl:choose>
            <xsl:when test="@name='Welcome'">
              <xsl:value-of select="''"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="../@default"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise> <!-- section or subsection -->
          <xsl:value-of select="../@name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="next">
      <xsl:choose>
        <xsl:when test="$level=1"> <!-- chapter -->
          <xsl:value-of select="following-sibling::mp:chapter[position()=1]/@name"/>
        </xsl:when>
        <xsl:when test="$level=2"> <!-- section -->
          <xsl:value-of select="following-sibling::mp:section[position()=1]/@name"/>
        </xsl:when>
        <xsl:otherwise> <!-- subsection -->
          <xsl:value-of select="following-sibling::mp:subsection[position()=1]/@name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="prev">
      <xsl:choose>
        <xsl:when test="$level=1"> <!-- chapter -->
          <xsl:value-of select="preceding-sibling::mp:chapter[position()=1]/@name"/>
        </xsl:when>
        <xsl:when test="$level=2"> <!-- section -->
          <xsl:value-of select="preceding-sibling::mp:section[position()=1]/@name"/>
        </xsl:when>
        <xsl:otherwise> <!-- subsection -->
          <xsl:value-of select="preceding-sibling::mp:subsection[position()=1]/@name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- Page table structure -->
    <a name="top_page"></a>
    <table class="page" border="0">
    <!-- Page Header -->
    <thead class="page"><tr>
    <td class="pad_TL">
        <xsl:if test="string-length($prev)>0">
        <a class="browse" href="{$prev}.{$out_ext}"><img class="browse" src="./files/images/Browse_Prev.gif" alt="Previous"/></a>
        </xsl:if>
    </td>
    <td class="pad_T">
        <xsl:if test="string-length($up)>0">
        <a class="browse" href="{$up}.{$out_ext}"><img class="browse" src="./files/images/Browse_Top.gif" alt="Up"/></a>
        </xsl:if>
    </td>
    <td class="pad_TR">
        <xsl:if test="string-length($next)>0">
        <a class="browse" href="{$next}.{$out_ext}"><img class="browse" src="./files/images/Browse_Next.gif" alt="Next"/></a>
        </xsl:if>
    </td>
    </tr></thead>
    <!-- Page Footer -->
    <tfoot class="page"><tr>
    <td class="pad_BL">
        <xsl:if test="string-length($prev)>0">
        <a class="browse" href="{$prev}.{$out_ext}"><img class="browse" src="./files/images/Browse_Prev.gif" alt="Previous"/></a>
        </xsl:if>
    </td>
    <td class="pad_B">
        <a class="browse" href="#top_page"> <img class="browse" src="./files/images/Browse_Top.gif" alt="Top of page"/></a>
    </td>
    <td class="pad_BR">
        <xsl:if test="string-length($next)>0">
        <a class="browse" href="{$next}.{$out_ext}"><img class="browse" src="./files/images/Browse_Next.gif" alt="Next"/></a>
        </xsl:if>
    </td>
    </tr></tfoot>
    <!-- Start Page Body -->
    <tbody><tr><td class="pad_L"/><td class="pad_Body">
    <!-- Insert body stuff -->
    <xsl:choose>
      <xsl:when test="$level=1"> <!-- chapter -->
        <!-- Version -->
        <div class="docversion">
          <xsl:value-of select="@version"/>
          <xsl:text> , </xsl:text>
          <!-- Put actual compile date in initial page -->
          <xsl:choose>
            <xsl:when test="@name='Welcome'">
              <xsl:call-template name="get-date"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@date"/>
            </xsl:otherwise>
          </xsl:choose>
        </div>
        <!-- Write title header -->
        <h1><a name="{@name}" title="{@description}"><xsl:value-of select="@title"/></a></h1>
      </xsl:when>
      <xsl:when test="$level=2"> <!-- section -->
        <h1><a name="{../@name}" href="{../@name}.{$out_ext}"><xsl:value-of select="../@title"/></a></h1>
        <!-- Write tabs here to allow current tab highlight -->
        <xsl:call-template name="build-tabs">
           <xsl:with-param name="sectnodes" select="'../mp:section'"/>
           <xsl:with-param name="currsection" select="@name"/>
        </xsl:call-template>
        <!-- Write header -->
        <h2><a name="{@name}" title="{@description}"><xsl:value-of select="@title"/></a></h2>
        <!-- print contents
        <xsl:call-template name="print-contents-toc">
            <xsl:with-param name="sectnodes" select="'./mp:subsection'"/>
            <xsl:with-param name="verbose" select="'yes'"/>
        </xsl:call-template> -->
      </xsl:when>
      <xsl:otherwise> <!-- subsection -->
        <h1><a name="{../../@name}" href="{../../@name}.{$out_ext}"><xsl:value-of select="../../@title"/></a></h1>
        <!-- Write tabs here to allow current tab highlight -->
        <xsl:call-template name="build-tabs">
           <xsl:with-param name="sectnodes" select="'../../mp:section'"/>
           <xsl:with-param name="currsection" select="../@name"/>
        </xsl:call-template>
        <h2><a name="{../@name}" href="{../@name}.{$out_ext}"><xsl:value-of select="../@title"/></a></h2>
        <!-- Write tabs here to allow current tab highlight -->
        <xsl:call-template name="build-tabs">
           <xsl:with-param name="sectnodes" select="'../mp:subsection'"/>
           <xsl:with-param name="currsection" select="@name"/>
        </xsl:call-template>
        <!-- Write header -->
        <h3><a name="{@name}" title="{@description}"><xsl:value-of select="@title"/></a></h3>
      </xsl:otherwise>
    </xsl:choose>
    <!-- Manage attribute-passed keywords -->
    <xsl:call-template name="HHmake-keywords">
        <xsl:with-param name="keywordlist" select="@keywords"/>
    </xsl:call-template>
    <!-- Now apply templates -->
    <xsl:apply-templates/>
    <!-- Inline frame with sections, put first child
    <iframe name="iframe_{@name}"
            src="{mp:section[position()=1]/@name}.{$out_ext}"
            frameborder="0" scrolling="auto"
            marginwidth="0" marginheight="0"
            width="100%" height="1000" align="middle">
    Inline frames not supported, change browser.
    </iframe> -->
    <!-- End Page Body -->
    </td><td class="pad_R"/></tr></tbody></table>
</xsl:template>

<!--
+++++++++++++++++++++++++++++++++++ `build-scrap' (build a scrap foil)
-->
<xsl:template name="build-scrap">
    <xsl:param name="node"/>
    <!-- Page table structure -->
    <a name="{@name}"></a>
    <table class="page" border="0">
    <!-- Page Header -->
    <thead class="page">
    <tr><td class="scrap_TL"></td><td class="scrap_T"></td><td class="scrap_TR"></td></tr></thead>
    <!-- Page Footer -->
    <tfoot class="page">
    <tr><td class="scrap_BL"></td><td class="scrap_B"></td><td class="scrap_BR"></td></tr></tfoot>  
    <!-- Start Scrap Body -->
    <tbody><tr><td class="scrap_L"/><td class="scrap_Body">
    <!-- title -->
    <xsl:if test="string(@title)">
        <h3><xsl:value-of select="@title"/></h3>
    </xsl:if>
    <!-- Now apply templates -->
    <xsl:choose>
        <xsl:when test="$node">
            <xsl:apply-templates select="$node"/>
        </xsl:when>
        <xsl:otherwise>
            <xsl:apply-templates/>
        </xsl:otherwise>
    </xsl:choose>
    <!-- End Scrap Body -->
    </td><td class="scrap_R"/></tr>
    </tbody></table>
</xsl:template>


<!--
+++++++++++++++++++++++++++++++++++ `print-html-header' (output htm header)
-->
<xsl:template name="print-html-header">
    <xsl:comment>
    <![CDATA[
    File created with a XSLT transformation: do not edit!
    ]]>
    <xsl:call-template name="get-date"/> - Matteo Gattanini
    </xsl:comment>
    <head>
        <title><xsl:value-of select="@title"/></title>
        <meta name="keywords" content="{@keywords}"/>
        <meta name="Description" content="Mplot html help page"/>
        <!-- Link to `Cascading Style Sheet' file -->
        <link rel="StyleSheet" href="./files/hlp_style.css" type="text/css"/>
        <!-- Link to script file (ehm, may work only in <body> -->
        <!-- <script language="Javascript" type="text/javascript" src="./files/hlp_scripts.js"><xsl:text> </xsl:text></script> -->
    </head>
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


<!--
+++++++++++++++++++++++++++++++++++ `get-date' (get a proper formatted date)
-->
<xsl:template name="get-date">
    <xsl:variable name="d" select="fn:day-from-date($cdate)"/>
    <xsl:variable name="m" select="fn:month-from-date($cdate)"/>
    <xsl:variable name="y" select="fn:year-from-date($cdate)"/>

    <!--<xsl:value-of select="$d"/><xsl:text> </xsl:text>-->
    <xsl:choose>
      <xsl:when test="$m=1">jan</xsl:when>
      <xsl:when test="$m=2">feb</xsl:when>
      <xsl:when test="$m=3">mar</xsl:when>
      <xsl:when test="$m=4">apr</xsl:when>
      <xsl:when test="$m=5">may</xsl:when>
      <xsl:when test="$m=6">jun</xsl:when>
      <xsl:when test="$m=7">jul</xsl:when>
      <xsl:when test="$m=8">aug</xsl:when>
      <xsl:when test="$m=9">sep</xsl:when>
      <xsl:when test="$m=10">oct</xsl:when>
      <xsl:when test="$m=11">nov</xsl:when>
      <xsl:when test="$m=12">dec</xsl:when>
      <xsl:otherwise>?</xsl:otherwise>
    </xsl:choose>
    <!--<xsl:text> </xsl:text>-->
    <xsl:value-of select="$y"/>
</xsl:template>


</xsl:transform> <!--================ End of File ================-->
