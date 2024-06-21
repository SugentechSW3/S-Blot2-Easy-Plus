#ifndef REAGENTPLACEHOLDERWIDGET_H
#define REAGENTPLACEHOLDERWIDGET_H

#include <QWidget>

//#define DW_COLOR {102,174,255}
//#define I_ANTIBODY_COLOR {49,62,72}
//#define F_ANTIBODY_COLOR {178,175,196}
//#define SUBSTRATE_COLOR {117,58,0}
//#define DILUENT_COLOR {27,169,40}
//#define I_ENZYME_COLOR {172,0,51}
//#define F_ENZYME_COLOR {238,79,61}
//#define WS_COLOR {1,80,211}

//#define DW_CENTER {120,120}
//#define I_ANTIBODY_CENTER {348,54}
//#define F_ANTIBODY_CENTER {462,54}
//#define SUBSTRATE_CENTER {576,54}
//#define DILUENT_CENTER {286,170}
//#define I_ENZYME_CENTER {402,170}
//#define F_ENZYME_CENTER {518,170}
//#define WS_CENTER {742,118}


class Ui_ReagentPlaceHolderWidget;


class ReagentPlaceHolderWidget : public QWidget
{
    Q_OBJECT
    struct privateStruct;

    struct privatReagenteHoldObject
    {
        QPixmap mRegentCheckImage;
        QPoint mRegentImagePoint;
        bool mIsSelected = false;
    };

public:
    explicit ReagentPlaceHolderWidget(QWidget *parent = nullptr);
    virtual ~ReagentPlaceHolderWidget();

    enum REAGENT_SELECT_IDX_ALLERGY
    {
        REAGENT_SELECT_IDX_ALLERGY_WS,
        REAGENT_SELECT_IDX_ALLERGY_SAMPLE_DILUENT,
        REAGENT_SELECT_IDX_ALLERGY_ANTIBODY,
        REAGENT_SELECT_IDX_ALLERGY_ENZYME,
        REAGENT_SELECT_IDX_ALLERGY_SUBSTRATE,
        REAGENT_SELECT_IDX_ALLERGY_DW,
        REAGENT_SELECT_IDX_ALLERGY_END
     };
    void setPlaceAllergyHolderStyleSheet(int select);
    void setMountAllergyIndex(int select, bool isMount);
protected:
    void paintEvent(QPaintEvent* event) override;
private:

    void initValiables();
    QPoint imageMovePoint(int x, int y);
private:
    QScopedPointer<Ui_ReagentPlaceHolderWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // REAGENTPLACEHOLDERWIDGET_H
